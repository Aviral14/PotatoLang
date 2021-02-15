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
        return characterClass::ALPHA;
    } else if (ch == '0') {
        return characterClass::ZERO;
    } else if (ch >= '1' && ch <= '9') {
        return characterClass::NUMERIC;
    } else if (ch == '.') {
        return characterClass::DOT;
    } else if (ch == ' ' || ch == '\t' || ch == '\n') {
        return characterClass::WHITESPACE;
    } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%') {
        return characterClass::ARITH;
    } else if (ch == '<' || ch == '>' || ch == '!' || ch == ':') {
        return characterClass::LOGIC;
    } else if (ch == '=') {
        return characterClass::EQU;
    } else if (ch == '{' || ch == '}' || ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == ';' || ch == ',') {
        return characterClass::DELIM;
    } else if (ch == '"') {
        return characterClass::QUOTE;
    } else if (ch == '&') {
        return characterClass::AND;
    } else if (ch == '|') {
        return characterClass::OR;
    } else if (ch == '$') {
        return characterClass::ENDOFFILE;
    } else {
        // invalid character;
        return characterClass::INVALID;
    }
}

/**
 * Returns the next state
 *
 * @param currState Current state
 * @param inputChar Input character read
 * @return Next state of DFA
 */
int DFA::transition(char inputChar) {
    characterClass column = identifyClass(inputChar);
    if (column == characterClass::INVALID) {
        return -1; // -1 represents error state
    }
    return transitionTable[curr_state][static_cast<int>(column)];
}
