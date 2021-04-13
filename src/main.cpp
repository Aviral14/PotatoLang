// Driver Program for the Frontend of ProtatoLang
#include "includes/lexer.hpp"
#include "includes/parser.hpp"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[]) {
    string code;
#ifndef TEST
    code = "";
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
    }
    ifstream f(argv[1]);
    if (!f) {
        cout << "FileNotFound:" << argv[1] << endl;
        return 0;
    }
    std::stringstream buffer;
    buffer << f.rdbuf();
    code = buffer.str();
    code.append("$");
#else
    if (argc != 1) {
        cout << "Usage: " << argv[0] << endl;
    }
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
    // Initialializing Lexer
    //Lexer lexer(code);

    // Instantiazing Parser
    Parser parser(code);

    // Beginning Parsing
    parser.startParsing();
    return 0;
}