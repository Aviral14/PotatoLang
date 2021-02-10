#include "dfa.hpp"
#include "config.hpp"

DFA::DFA() {
    prev_state = curr_state = 0;
}

auto DFA::identifyClass(char ch) {
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
        return alpha;
    } else if (ch == '0') {
        return zero;
    } else if (ch >= '1' && ch <= '9') {
        return numeric;
    } else if (ch == '.') {
        return dot;
    } else if (ch == ' ' || ch == '\t' || ch == '\n') {
        return whitespace;
    } else if (ch == '$') {
        return eof;
    } else {
        // invalid character;
        return invalid;
    }
}

int DFA::transition(int state, char ch) {
    enum characterClass column = identifyClass(ch);
    if (column == invalid) {
        return -1; // -1 represents error state
    }
    return transitionTable[curr_state][column];
}
