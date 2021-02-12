#include "lexer.hpp"
#include <iostream>
#include <string>
#include <utility>

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

/**
 * Handles the error
 * TODO: Return Error code if necessary
 *
 */
int Lexer::handleStringLiteral() {
    //handle string literal separately
    while (*(++fp) != '"') {
        if (*fp == '\\' &&
            (*(fp + 1) != 'n' || *(fp + 1) != 'r' || *(fp + 1) != 't' || *(fp + 1) != '\\')) {
            handleError();
        } else if (*fp == '$') {
            //oof get REKT
            handleError();
            return 1;
        }
    }
    fp++;
    dfa.prev_state = 99;
    dfa.curr_state = 0;
    return 0;
}

int Lexer::handleAndsOrs() {
    //handle || and && separately
    string oper(fp, fp + 2);
    if (oper != "&&" && oper != "||") {
        handleError();
        return 1;
    }
    fp += 2;
    dfa.prev_state = 98;
    dfa.curr_state = 0;
    return 0;
}

int Lexer::handleError() {
    // seek till next whitespace
    if (*fp == '\\') {
        sp = fp + 1;
        cout << "Bad escape sequence " << string(fp, fp + 2)
             << "\nIn Line number: " << line << endl;
        return 2;
    } else if (*fp == '&' || *fp == '|') {
        sp = fp + 1;
        cout << "Illegal Operator " << string(fp, fp + 1)
             << "\nIn Line number: " << line << endl;
        return 3;
    } else if (*fp == '$') {
        cout << "Illegal Termination"
             << "\nIn Line number: " << line << endl;
    } else {
        cout << "Invalid Token found"
             << "\nIn Line number: " << line << endl;
    }
    while (*fp != ' ' && *fp != '\n' && *fp != '\t' && *fp != '$') {
        fp++;
    }
    sp = fp;
    fp--; // Retract fp
    dfa.curr_state = 0;
    return 1;
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
        dfa.curr_state = dfa.transition(dfa.curr_state, *fp);

        if (dfa.curr_state == 100) {
            return {"EOF", "$", 0}; // Reached End State
        } else if (dfa.curr_state == 99) {
            if (handleStringLiteral())
                continue;
        } else if (dfa.curr_state == 98) {
            if (handleAndsOrs())
                continue;
        } else if (dfa.curr_state == -1) {
            handleError();
            dfa.curr_state = 0; // Recover from Error State
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
            switch (dfa.prev_state) {
            case 1:
                token = "identifier";
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
            case 98:
                token = "operator";
                break;
            case 99:
                token = "string literal";
                break;
            }
            lexeme = getString();
            sp = fp;
            fp--; // Retract fp
            return {token, lexeme,
                    0}; // Line Number has been hardcoded as 0 for now
        }
    }
}
