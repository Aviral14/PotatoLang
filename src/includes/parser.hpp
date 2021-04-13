#pragma once

#include "config.hpp"
#include "lexer.hpp"
#include "table.hpp"
#include <stack>
using std::stack;

class Parser {
    vector<State> states; // The Parse Table
    Grammar grammar;      // PotatoLang Grammar
    Lexer lex;            // PotatoLang Lexer Instance
    string symbol;        // Symbols of Production Rules
    string prevSymbol;    // Memory of Symbol
    int currState;        // Parser state
    stack<string> st;     // Parser Stack
    int line;             // The current line being parsed

    void transit(string, string);
    void handleShift(string);
    void handleReduce(string);
    void handleGOTO(string);
    bool handleError(exceptionClass);

  public:
    Parser(string &);
    void startParsing();
};
