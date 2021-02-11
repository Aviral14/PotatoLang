#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <utility>

using namespace std;

const int MAX_CODE_LENGTH = 1000;
const int MAX_LEXEME_LENGTH = 255;

// Structure for storing token, lexeme pairs from lexer
struct lexResult {
    string token;
    string lexeme;
    int lineNumber;
};

/**
 * Character Classes
 * invalid character- all characters which are not specified in Regular Grammar
 * alpha- a-z|A-Z|_
 * zero- 0
 * numeric- 1-9
 * whitespace- ' '|'\n'|'\t'
 * eof- $
 */

enum characterClass {
    INVALID = -1,
    ALPHA = 0,
    ZERO,
    NUMERIC,
    DOT,
    WHITESPACE,
    ENDOFFILE
};

/**
 * Transition Table-
 *          alpha   zero   numeric   dot   whitespace   eof
 * state 0    1       3       2       -1       0        100
 * state 1    1       1       1       -1       0         0
 * state 2    0       2       2        4       0         0
 * state 3    0       0       0        4       0         0
 * state 4    0       4       4       -1       0         0
 *
 * state -1 represents inalid transition and state 100 reperesents end state
 */

const int transitionTable[5][6] = {{1, 3, 2, -1, 0, 100},
                                   {1, 1, 1, -1, 0, 0},
                                   {0, 2, 2, 4, 0, 0},
                                   {0, 0, 0, 4, 0, 0},
                                   {0, 4, 4, -1, 0, 0}};

#endif
