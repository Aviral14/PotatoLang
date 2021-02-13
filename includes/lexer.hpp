#pragma once

#include "dfa.hpp"
#include <string>
#include <utility>

using namespace std;
/*! \class Lexer
    \brief Lexer class for handling lexical analysis
*/
class Lexer {
    string::iterator sp; /*!< start pointer */
    string::iterator fp; /*!< forward pointer */
    string text;
    string lexeme;
    int line;
    DFA dfa;

    string getString();                              /*!< Fetches string between the Start Pointer and the
                           Forward Pointer. */
    exceptionClass handleError(enum exceptionClass); /*!< Handle Error if any. */

    int handleStringLiteral(); /*!< Handle mini-dfa for String Literal */

  public:
    Lexer(string code);

    /**
     * Iterates over the provided code
     * Tokenizes the code adhering to the Specified Regular Grammar
     * @return lexResult, a {token,lexeme,line number} struct
     */
    lexResult getLexeme();
};
