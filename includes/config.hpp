#pragma once // works as an include guard. Not a CPP standard but almost all
             // compilers support it and is much faster since files don't have
             // to be reopened

#include <string>
#include <utility>

using std::string;

const int MAX_CODE_LENGTH = 1000;
const int MAX_LEXEME_LENGTH = 255;

/*! \struct LexResult
    \brief Store the result of lexer as a pair
    Structure for storing token, lexeme pairs from lexer
*/
struct lexResult {
    string token;
    string lexeme;
    int lineNumber;
};

/*! \enum characterClass
    \brief Character Classes
    invalid character- all characters which are not specified in Regular Grammar
    alpha- a-z|A-Z|_
    zero- 0
    numeric- 1-9
    whitespace- ' '|'\n'|'\t'
    eof- $
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