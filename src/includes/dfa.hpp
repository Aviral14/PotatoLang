#pragma once
#include "types.hpp"

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
    int transition(char);

  private:
    characterClass identifyClass(char);
};
