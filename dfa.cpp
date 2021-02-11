#include "dfa.hpp"
#include "config.hpp"

DFA::DFA() {
    prev_state = curr_state = 0;
}

characterClass DFA::identifyClass(char ch) {
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
        return ALPHA;
    } else if (ch == '0') {
        return ZERO;
    } else if (ch >= '1' && ch <= '9') {
        return NUMERIC;
    } else if (ch == '.') {
        return DOT;
    } else if (ch == ' ' || ch == '\t' || ch == '\n') {
        return WHITESPACE;
    } else if (ch == '$') {
        return ENDOFFILE;
    } else {
        // invalid character;
        return INVALID;
    }
}

int DFA::transition(int state, char ch) {
    enum characterClass column = identifyClass(ch);
    if (column == INVALID) {
        return -1; // -1 represents error state
    }
    return transitionTable[curr_state][column];
}
