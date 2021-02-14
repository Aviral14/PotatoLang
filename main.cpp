// Driver program for PotatoLang Programming Language Compiler
#include "lexer.hpp"
#include <iostream>
#include <utility>

using namespace std;

int main() {
    string code;

#ifndef TEST
    code = "";
    std::string line;
    while (std::getline(std::cin, line)) {
        code.append(line);
        cout << line;
        code.append("\n");
    }
    code.append("$");
#else
    code = "/* A program to compute factorials */\n"
           "int fact(int n) {\n"
           "    if (n <= 1)\n"
           "        return 1;\n"
           "    else\n"
           "        return n * fact(n - 1);\n"
           "}\n"
           "void main(void) {\n"
           "    int x;\n"
           "    x = 1;\n"
           "    while (x <= 10) {\n"
           "        write(x);\n"
           "        write(fact(x));\n"
           "        writeln();\n"
           "        x = x + 1;\n"
           "    }\n"
           "}\n"
           "$";
#endif
    Lexer lex(code);
    lexResult res;

    while (res.token != "EOF") {
        res = lex.getLexeme();
        cout << res.token << " " << res.lexeme << " " << res.lineNumber << endl;
    }
}
