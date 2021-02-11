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
}

/**
 * Returns the lexeme
 * @return str, a string containing the lexeme
 */
string Lexer::getString() {
    /**
     * Boring C-style method
       char str[MAX_LEXEME_LENGTH];
       for (int i = 0; i < fp - sp; i++) {
           str[i] = *(sp + i);
       }
       str[fp - sp] = '\0';
       return (string)str;
     */
    // vs chad C++ style string
    string str(sp, fp);
    return str;
}

/**
 * Handles the error
 * TODO: Return Error code if necessary
 *
 */
int Lexer::handleError() {
    // seek till next whitespace
    while (*fp != ' ' && *fp != '\n' && *fp != '\t' && *fp != '$') {
        fp++;
    }
    sp = fp;
    fp--; // Retract fp
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
        }
        if (dfa.curr_state == -1) {
            cout << "Error: Invalid Token found!" << endl;
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
            case 4:
                token = "floating point literal";
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
