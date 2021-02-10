#include "lexer.hpp"
#include <iostream>
#include <string>
#include <utility>

using namespace std;

Lexer::Lexer(string code) {
    text = code;
    sp = text.begin();
    fp = text.end();
}

string Lexer::getString() {
    int i = 0;
    char str[MAX_LEXEME_LENGTH];
    for (; i < fp - sp; i++) {
        str[i] = *(sp + i);
    }
    str[i] = '\0';
    return (string)str;
}

void Lexer::handleError() {
    while (*fp != ' ' && *fp != '\n' && *fp != '\t' && *fp != '$') {
        fp++;
    }
    sp = fp;
    fp--; // Retract fp
}

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
