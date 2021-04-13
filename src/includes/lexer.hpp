#pragma once

#include "dfa.hpp"
#include <string>
#include <utility>

using std::string;

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

    string getString();               /*!< Fetches string between the Start Pointer and the
                           Forward Pointer. */
    void handleError(exceptionClass); /*!< Handle Error if any. */

    int handleStringLiteral(); /*!< Handle mini-dfa for String Literal */

    int handleComment(); /*!< Handle Comment block */

  public:
    Lexer(string code);

    Lexer() = default;
    /**
     * Iterates over the provided code
     * Tokenizes the code adhering to the Specified Regular Grammar
     * @return lexResult, a {token,lexeme,line number} struct
     */
    lexResult getLexeme();
};
