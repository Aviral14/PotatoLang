#include "includes/lexer.hpp"
#include "includes/config.hpp"
#include "includes/types.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::find;

/**
 * Constructor
 * @param code Full code to be lexed
 */
Lexer::Lexer(string code) {
    text = code;
    sp = text.begin();
    fp = text.end();
    line = 1;
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
            /* \r \n \t \\ \" */
            if (*fp == '\\' && *(fp + 1) != 'n' && *(fp + 1) != 'r' && *(fp + 1) != 't' && *(fp + 1) != '\\' && *(fp + 1) != '"') {
                throw exceptionClass::BAD_ESCAPE_SEQUENCE;
            } else if (*fp == '\\') {
                fp++;
            } else if (*fp == '$' || *fp == '\r' || *fp == '\n') {
                //oof get REKT
                throw exceptionClass::BAD_TERMINATOR;
            }
        } catch (exceptionClass e) {
            handleError(e);
            if (e == exceptionClass::BAD_TERMINATOR)
                return 1;
        }
    }
    fp++;
    // A case that couldn't be handled more elegantly without messing up some other part of the code
    if (*fp == '\n')
        line++;
    dfa.prev_state = 99;
    dfa.curr_state = 0;
    return 0;
}

int Lexer::handleComment() {
    while (*(fp) != '$') {
        if (*(fp) == '\n') {
            line++; // Increment line counter on encountering a newline.
        }
        if (*(fp) == '*' && (fp + 1) != text.end() && *(fp + 1) == '/') {
            fp = fp + 1;
            sp = fp + 1;
            dfa.curr_state = 0;
            return 0;
        }
        fp++;
    }

    // Unterminated comment block. May lead to all code after the comment
    // initialization to be ignored. So, throw an error.
    sp = fp;
    fp--; // Retract fp;
    throw exceptionClass::UNTERMINATED_COMMENT_BLOCK;
}

/**
 * Handles the error
 * @return Error code
 *
 */
void Lexer::handleError(exceptionClass ec) {
    bool seekToQuotation = false;
    if (ec == exceptionClass::BAD_ESCAPE_SEQUENCE) {
        sp = fp + 1;
        cout << "Bad escape sequence \"" << string(fp, fp + 2) << "\""
             << "\n\tIn Line number: " << line << endl;
        seekToQuotation = true;

    } else if (ec == exceptionClass::BAD_TERMINATOR) {
        cout << "Bad terminator encountered: ";
        switch (*fp) {
        case '\n':
            cout << "newline (\\n)"
                 << "\n\tIn Line number: " << line << endl;
            line++;
            sp = fp + 1;
            dfa.curr_state = dfa.prev_state = 0; //discard the string entirely.
            /*if we encounter a newline, it only makes sense to terminate the 
            string right here*/
            return;
        case '\r':
            cout << "carriage return (\\r)"
                 << "\n\tIn Line number: " << line << endl;
            seekToQuotation = true;
            break;
        case '$':
            sp = fp;
            cout << "EOF"
                 << "\n\tIn Line number: " << line << endl;
        }

    } else if (ec == exceptionClass::UNTERMINATED_COMMENT_BLOCK) {
        cout << "Unterminated comment block found on line " << line << endl;
    } else if (*(fp - 1) == '&' || *(fp - 1) == '|') {
        cout << "Invalid Token found \"" << string(fp - 1, fp) << "\""
             << "\n\tIn Line number: " << line << endl;
        sp = fp;
        fp--;
        dfa.curr_state = 0;
        return;
    } else {
        cout << "Invalid Token found \"" << string(fp, fp + 1) << "\""
             << "\n\tIn Line number: " << line << endl;
    }

    if (seekToQuotation) {
        while ((!(*fp == '\"' && *(fp - 1) != '\\') && *fp == '\n') && *fp != '$') {
            fp++;
        }
        if (*fp == '$') { //if the mofo decided to make an error and not terminate.
            sp = fp;
            fp--;
            dfa.curr_state = 0;
            cout << "EOF"
                 << "\n\tIn Line number: " << line << endl;
        } else if (*fp == '\n') {
            cout << "Unexpected newline (\\n)"
                 << "\n\tIn Line number: " << line << endl;
            line++;
            sp = fp + 1;
            dfa.curr_state = dfa.prev_state = 0; //discard the string entirely.
        } else {
            sp = fp + 1;
            dfa.curr_state = dfa.prev_state = 0; //discard the string entirely.
        }
        return;
    }

    // seek till next whitespace
    while (*fp != ' ' && *fp != '\n' && *fp != '\t' && *fp != '$') {
        if (*fp == '\n') {
            line++; // Increment line counter on encountering a newline.
        }
        fp++;
    }
    sp = fp;
    fp--; // Retract fp
    dfa.curr_state = 0;
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

        if (*fp == '\n') {
            line++; // Increment line counter on encountering a newline.
        }

        try {

            if (dfa.curr_state == 100) {
                return {"$", "$", line}; // Reached End State
            } else if (dfa.curr_state == 99) {
                if (handleStringLiteral())
                    continue;
            }
            // Check if a comment is possible
            else if (*fp == '/' && (fp + 1) != text.end() && *(fp + 1) == '*') {
                handleComment();
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

            if (*fp == '\n') {
                line--; // Decrement line count as we are retracting back.
            }

            fp--; // Retract fp
            switch (dfa.prev_state) {
            case 1:
                if (find(keywords.begin(), keywords.end(), lexeme) != keywords.end()) {
                    token = "keyword";
                } else {
                    token = "id";
                }
                break;
            case 2:
            case 3:
                token = "int";
                break;
            case 5:
                token = "flt";
                break;
            case 6:
            case 7:
            case 8:
            case 10:
            case 13:
            case 15:
                token = "op";
                break;
            case 9:
            case 11:
                token = lexeme;
                break;
            case 99:
                token = "str";
                break;
            }
            if (token == "keyword") {
                if (std::find(datatype.begin(), datatype.end(), lexeme) != datatype.end()) {
                    //it is a datatype
                    token = "dat";
                } else if (lexeme == "true" || lexeme == "false") {
                    // It is a boolean literal
                    token = "boolean";
                } else {
                    // It is a statement keyword
                    token = lexeme;
                }
            }
            return {
                token,
                lexeme,
                line};
        }
    }
}
