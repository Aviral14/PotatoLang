#ifndef LEXER_H
#define LEXER_H

#include "config.hpp"
#include "dfa.hpp"
#include <string>
#include <utility>

using namespace std;

class Lexer {
    string::iterator sp; // start pointer
    string::iterator fp; // forward pointer
    string text;
    string lexeme;
    DFA dfa;

    // Fetches string between the Start Pointer and the Forward Pointer
    string getString();
    int handleError();

  public:
    Lexer(string code);

    /**
     * Iterates over the provided code
     * Tokenizes the code adhering to the Specified Regular Grammar
     * Returns <<token,lexeme>,line number> pair
     */
    lexResult getLexeme();
};

#endif
