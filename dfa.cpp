#include "dfa.hpp"

DFA::DFA() {
    prev_state = curr_state = 0;
}
/**
 * Identifies the character class for each character fed
 *
 * @param ch character to be identified
 * @return character class of character
 */
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
    } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%') {
        return ARITH;
    } else if (ch == '<' || ch == '>' || ch == '!' || ch == ':') {
        return LOGIC;
    } else if (ch == '=') {
        return EQU;
    } else if (ch == '{' || ch == '}' || ch == '(' || ch == ')' || ch == ';' || ch == ',') {
        return DELIM;
    } else if (ch == '"') {
        return QUOTE;
    } else if (ch == '&') {
        return AND;
    } else if (ch == '|') {
        return OR;
    } else if (ch == '$') {
        return ENDOFFILE;
    } else {
        // invalid character;
        return INVALID;
    }
}

/**
 * Returns the next state
 *
 * @param currState Current state
 * @param inputChar Input character read
 * @return Next state of DFA
 */
int DFA::transition(int currState, char inputChar) {
    enum characterClass column = identifyClass(inputChar);
    if (column == INVALID) {
        return -1; // -1 represents error state
    } else if (column == AND || column == OR) {
        return 98;
    } else if (column == QUOTE) {
        return 99;
    }
    return transitionTable[curr_state][column];
}
