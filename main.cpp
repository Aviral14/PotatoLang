// Driver program for PotatoLang Programming Language Compiler
#include "lexer.hpp"
#include <iostream>
#include <utility>

using namespace std;

int main() {
    string code;
#if TEST == 1
    /*
    code = "  abcdiu kopk90 9000 7821 0.718 0 fui_8932 00.456 9.120 00.456 00 "
           "abc90 90_ 90   _fnsi + && - / <= >= == := float$";
*/
    code = "\"Hello \\r I'm a fool \\b\" are you/ one int. 3.12 00.4++34 {2<=} &will you |float || sink # while I'm dead? $";
#else
    cout << "Enter Your Code" << endl;
    getline(cin, code, '\n'); // Change delimiter to EOF later
    code.append("$");
#endif

    Lexer lex(code);
    lexResult res;

    while (res.token != "EOF") {
        res = lex.getLexeme();
        cout << res.token << " " << res.lexeme << " " << res.lineNumber << endl;
    }
}
