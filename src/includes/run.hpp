#include "lexer.hpp"
#include "parser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::stringstream;

/** Start the compiler **/
int run(int argc, char *argv[]);