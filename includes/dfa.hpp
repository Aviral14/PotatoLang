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

  private:
    /**
     * Transition Table-
     *          alpha   zero   numeric   dot   whitespace   eof
     * state 0    1       3       2       -1       0        100
     * state 1    1       1       1       -1       0        0
     * state 2    0       2       2        4       0        0
     * state 3    0       0       0        4       0        0
     * state 4    0       4       4       -1       0        0
     *
     * state -1 represents inalid transition and state 100 reperesents end state
     */

    const int transitionTable[5][6] = {{1, 3, 2, -1, 0, 100},
                                       {1, 1, 1, -1, 0, 0},
                                       {0, 2, 2, 4, 0, 0},
                                       {0, 0, 0, 4, 0, 0},
                                       {0, 4, 4, -1, 0, 0}};

    characterClass identifyClass(char);
};
