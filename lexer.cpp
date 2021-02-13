#include "lexer.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <utility>

using std::find;
/**
 * Constructor
 * @param code Full code to be lexed
 */
Lexer::Lexer(string code) {
    text = code;
    sp = text.begin();
    fp = text.end();
    line = 0;
}

/**
 * Returns the lexeme
 * @return a string containing the lexeme
 */
string Lexer::getString() {
    return string(sp, fp);
}

int Lexer::handleStringLiteral() {
    //handle string literal separately
    while (*(++fp) != '"') {
        try {
            if (*fp == '\\' && *(fp + 1) != 'n' && *(fp + 1) != 'r' && *(fp + 1) != 't' && *(fp + 1) != '\\' && *(fp + 1) != '"') {
                throw exceptionClass::BAD_ESCAPE_SEQUENCE;
            } else if ((*fp == '\r' || *fp == '\n' || *fp == '"')) {
                throw exceptionClass::BAD_CHARACTER;
            } else if (*fp == '$') {
                //oof get REKT
                throw exceptionClass::BAD_TERMINATOR;
                return 1;
            }
        } catch (exceptionClass e) {
            handleError(e);
        }
    }
    fp++;
    dfa.prev_state = 99;
    dfa.curr_state = 0;
    return 0;
}

/**
 * Handles the error
 * @return Error code
 *
 */
exceptionClass Lexer::handleError(exceptionClass ec) {

    if (ec == exceptionClass::BAD_ESCAPE_SEQUENCE) {
        sp = fp + 1;
        cout << "Bad escape sequence \"" << string(fp, fp + 2) << "\""
             << "\n\tIn Line number: " << line << endl;
        return ec;
    } else if (ec == exceptionClass::BAD_CHARACTER) {
        sp = fp;
        cout << "Bad escape sequence \"" << string(fp, fp + 2) << "\""
             << "\n\tIn Line number: " << line << endl;
        return ec;
    } else if (ec == exceptionClass::BAD_TERMINATOR) {
        cout << "Uxexepcted EOF"
             << "\n\tIn Line number: " << line << endl;
    } else if (*(fp - 1) == '&' || *(fp - 1) == '|') {
        cout << "Invalid Token found \"" << string(fp - 1, fp) << "\""
             << "\n\tIn Line number: " << line << endl;
        sp = fp;
        fp--;
        dfa.curr_state = 0;
        return ec;
    } else {
        cout << "Invalid Token found \"" << string(fp, fp + 1) << "\""
             << "\n\tIn Line number: " << line << endl;
    }

    // seek till next whitespace
    while (*fp != ' ' && *fp != '\n' && *fp != '\t' && *fp != '$') {
        fp++;
    }
    sp = fp;
    fp--; // Retract fp
    dfa.curr_state = 0;
    return ec;
}

/**
 * Function to retrieve the lexeme and return it
 * Side Effects on sp and fp
 *
 */
lexResult Lexer::getLexeme() {
    string token, lexeme;
    while (true) {
        if (fp == text.end()) {
            fp = text.begin(); // if fp is uninitialized , move fp to start of
                               // code
        } else {
            fp++;
        }
        dfa.prev_state = dfa.curr_state;
        dfa.curr_state = dfa.transition(*fp);
        try {
            if (dfa.curr_state == 100) {
                return {"EOF", "$", 0}; // Reached End State
            } else if (dfa.curr_state == 99) {
                if (handleStringLiteral())
                    continue;
            } else if (dfa.curr_state == -1) {
                throw exceptionClass::BAD_TOKEN;
            }
        } catch (exceptionClass e) {
            handleError(e);
            continue;
        }
        // if prev_state = curr_state = 0, we have encountered a whitespace
        if (!dfa.prev_state && !dfa.curr_state) {
            sp++;
            continue; // ignore whitespace
        }

        // if curr_state = 0 and prev_state != 0 , we have reached the start
        // state and a lexeme has been identified
        if (!dfa.curr_state) {

            lexeme = getString();
            sp = fp;
            fp--; // Retract fp
            switch (dfa.prev_state) {
            case 1:
                if (find(keywords.begin(), keywords.end(), lexeme) != keywords.end()) {
                    token = "keyword";
                } else {
                    token = "identifier";
                }
                break;
            case 2:
            case 3:
                token = "integer literal";
                break;
            case 5:
                token = "floating point literal";
                break;
            case 6:
            case 7:
            case 8:
            case 11:
            case 13:
                token = "operator";
                break;
            case 9:
                token = "delimiter";
                break;
            case 99:
                token = "string literal";
                break;
            }
            return {token, lexeme,
                    0}; // Line Number has been hardcoded as 0 for now
        }
    }
}
