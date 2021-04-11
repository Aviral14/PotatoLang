#pragma once
/* works as an include guard. Not a CPP standard but almost all   \
              compilers support it and is much faster since files don't have \
              to be reopened */

#include <array>
#include <string>
#include <utility>

using std::array;
using std::string;

const int MAX_CODE_LENGTH = 1000;
const int MAX_LEXEME_LENGTH = 255;

/*! 
    \brief Keyword list
 */
const array<string, 10> keywords = {"int",
                                    "float",
                                    "boolean",
                                    "string",
                                    "while",
                                    "until",
                                    "if",
                                    "else",
                                    "true",
                                    "false"};

const array<string, 4> datatype = {"int",
                                   "float",
                                   "boolean",
                                   "string"};
/**
     \brief Transition Table-
          char->  alpha   zero   numeric   dot   whitespace  arith   log  equ  delim    "      &      |     EOF 
       0 state pri  1,      3,      2,      -1,      0,       6,      7,    9,   11,    99,    12,   14,   100 zeros 
       1 state al   1,      1,      1,      -1,      0,       0,      0,    0,    0,    0 ,    0 ,    0,    0
       2 state num  0,      2,      2,       4,      0,       0,      0,    0,    0,    0 ,    0 ,    0,    0
       3 state zer  0,      0,      0,       4,      0,       0,      0,    0,    0,    0 ,    0 ,    0,    0
       4 state dot -1,      5,      5,      -1,     -1,      -1,     -1,   -1,   -1,   -1 ,   -1 ,   -1,   -1 //since invalid float literal
       5 state flo  0,      5,      5,      -1,      0,       0,      0,    0,    0,    0 ,    0 ,    0,    0
       6 state ari  0,      0,      0,      -1,      0,       0,      0,    0,    0,    0 ,    0 ,    0,    0
       7 state log  0,      0,      0,      -1,      0,       0,      0,    8,    0,    0 ,    0 ,    0,    0
       8 state lo2  0,      0,      0,      -1,      0,       0,      0,    0,    0,    0,     0,     0,    0
       9 state equ  0,      0,      0,      -1,      0,       0,      0,    8,    0,    0 ,    0 ,    0,    0
       10 state eq2 0,      0,      0,      -1,      0,       0,      0,    0,    0,    0,     0,     0,    0
       11 state del 0,      0,      0,      -1,      0,       0,      0,    0,    0,    0 ,    0 ,    0,    0
       12 state &  -1,     -1,     -1,      -1,     -1,      -1,     -1,   -1,   -1,   -1 ,    11,   -1,   -1 
       13 state &2  0,      0,      0,      -1,      0,       0,      0,    0,    0,    0 ,    0 ,    0,    0                  
       14 state |  -1,     -1,     -1,      -1,     -1,      -1,     -1,   -1,   -1,   -1 ,   -1 ,   13,   -1
       15 state |2  0,      0,      0,      -1,      0,       0,      0,    0,    0,    0 ,    0 ,    0,    0
       state -1 represents inalid transition and state 100 reperesents end state
       NOTE: due to the large size of the transition table, the mini-dfa
       for string literals has been implemented separately, and that is represented by
       state 99 in DFA::transition

       NOTE: Keywords have been implemented via string comparison
     */
const int transitionTable[16][13] = {
    {1, 3, 2, -1, 0, 6, 7, 9, 11, 99, 12, 14, 100},       //0
    {1, 1, 1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},             //1
    {0, 2, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0},              //2
    {0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0},              //3
    {-1, 5, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},   //4
    {0, 5, 5, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},             //5
    {0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},             //6
    {0, 0, 0, -1, 0, 0, 0, 8, 0, 0, 0, 0, 0},             //7
    {0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},             //8
    {0, 0, 0, -1, 0, 0, 0, 8, 0, 0, 0, 0, 0},             //9
    {0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},             //10
    {0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},             //11
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 11, -1, -1}, //12
    {0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},             //13
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 13, -1}, //14
    {0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0}};            //15

const string slr_json_location = "slr_info.json";
