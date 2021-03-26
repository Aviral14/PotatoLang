#pragma once
#include <unordered_map>
#include <vector>

using namespace std;

struct Rule {
    size_t index;
    string nonterminal;
    vector<string> pattern;
    vector<string> development;
};

struct Action {
    string actionType;
    int actionValue;
};

struct State {
    size_t index;

    // This is a vector of actions. If there is no conflict ( such as a s/r
    // conflict or an r/r conflict ) then there will be only one action,
    // accessible using index 0
    unordered_map<string, vector<Action>> actions;
};

struct Grammar {
    vector<string> alphabet;
    vector<string> nonterminals;
    vector<string> terminals;
    vector<Rule> rules;
    unordered_map<string, vector<string>> firsts;
    unordered_map<string, vector<string>> follows;
    string axiom;
};