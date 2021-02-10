#ifndef DFA_H
#define DFA_H

class DFA {
  public:
    int curr_state;
    int prev_state;

    DFA();
    int transition(int, char);

  private:
    auto identifyClass(char);
};

#endif
