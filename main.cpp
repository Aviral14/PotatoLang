// Driver program for PotatoLang Programming Language Compiler
#include "config.hpp"
#include "lexer.hpp"
#include <iostream>
#include <utility>

using namespace std;

int main() {
    cout << "Enter Your Code" << endl;
    string code;
    getline(cin, code, '\n'); // Change delimiter to EOF later
    code.append("$");
    // For testing use- "  abcdiu kopk90 9000 7821 0.718 0 fui_8932 00.456 9.120
    // 00.456 00 abc90 90_ 90   _fnsi"

    Lexer lex(code);
    lexResult res;

    while (res.token != "EOF") {
        res = lex.getLexeme();
        cout << res.token << " " << res.lexeme << " " << res.lineNumber << endl;
    }
}
