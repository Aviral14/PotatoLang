#pragma once
#include "config.hpp"
/*!
  \class DFA
  \brief A DFA
  DFA class definition to maintain a DFA for lexical analysis
  Stores the current and previous state
  Transition table as defined in
 */
class DFA {
  public:
    int curr_state;
    int prev_state;

    DFA();
    int transition(int, char);
    float a = .123;

  private:
    /**
     * Transition Table-
                alpha   zero   numeric   dot   whitespace  arith   log  equ  delim  "   EOF 
       state 0    1       3       2       -1       0        6       7     8     9    99  100, zeros 
       state 1    1       1       1       -1       0        0       0     0     0    0    0
       state 2    0       2       2        4       0        0       0     0     0    0    0
       state 3    0       0       0        4       0        0       0     0     0    0    0
       state 4    0       5       5       -1      -1       -1      -1    -1    -1   -1    -1 //since invalid float literal
       state 5    0       5       5       -1       0        0       0     0     0    0    0
       state 6    0       0       0       -1       0        0      -1     0    -1   -1    0
       state 7    0       0       0       -1       0        0      -1     7    -1   -1    0
       state 8    0       0       0       -1       0        0      -1     8    -1    0    0
       state 9    0       0       0       -1       0        0       0     0     0    0    0
     * state -1 represents inalid transition and state 100 reperesents end state
     * NOTE: due to the large size of the transition table, the mini-dfa
     * for string literals and {&&,||} has been implemented separately, and that is represented by
     * state 99 and 98 in DFA::transition
     * State 5 is not to be confused with the WHITESPACE state, it's an intermediary to 
     * distinguish an illegal floating point number
     * from a legal one
     */

    int transitionTable[10][11] = {
        {1, 3, 2, -1, 0, 6, 7, 8, 9, 99, 100},
        {1, 1, 1, -1, 0, 0, 0, 0, 0, 0, 0},
        {0, 2, 2, 4, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0},
        {0, 5, 5, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, 5, 5, -1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, -1, 0, 0, -1, 0, -1, -1, 0},
        {0, 0, 0, -1, 0, 0, -1, 7, -1, -1, 0},
        {0, 0, 0, -1, 0, 0, -1, 8, -1, -1, 0},
        {0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0},

    };
    characterClass identifyClass(char);
};
