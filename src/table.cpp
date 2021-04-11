#include "includes/table.hpp"

using namespace std;

Grammar getGrammar(string jsonString) {
    char *json = new char[jsonString.length() + 1];
    strcpy(json, jsonString.c_str());
    Document document;
    document.Parse(json);

    Grammar g;
    const Value &grammar = document["grammar"];

    for (auto &v : grammar["alphabet"].GetArray()) {
        g.alphabet.push_back(v.GetString());
    }

    for (auto &v : grammar["nonterminals"].GetArray()) {
        g.nonterminals.push_back(v.GetString());
    }

    for (auto &v : grammar["terminals"].GetArray()) {
        g.terminals.push_back(v.GetString());
    }

    const Value &rules = grammar["rules"];

    for (auto &rule : rules.GetArray()) {
        Rule currRule;
        currRule.index = rule["index"].GetInt();
        currRule.nonterminal = string(rule["nonterminal"].GetString());

        for (auto &p : rule["pattern"].GetArray()) {
            currRule.pattern.push_back(p.GetString());
        }

        for (auto &d : rule["development"].GetArray()) {
            currRule.pattern.push_back(d.GetString());
        }

        g.rules.push_back(currRule);
    }

    for (string nt : g.nonterminals) {
        vector<string> follows;
        for (auto &e : grammar["follows"][nt.c_str()].GetArray()) {
            follows.push_back(string(e.GetString()));
        }
        g.follows[nt] = follows;
    }

    for (string nt : g.nonterminals) {
        vector<string> firsts;
        for (auto &e : grammar["firsts"][nt.c_str()].GetArray()) {
            firsts.push_back(string(e.GetString()));
        }

        g.firsts[nt] = firsts;
    }

    g.axiom = string(grammar["axiom"].GetString());

    return g;
}

vector<State> getStates(string jsonString, Grammar g) {
    char *json = new char[jsonString.length() + 1];
    strcpy(json, jsonString.c_str());
    Document document;
    document.Parse(json);

    vector<State> states;

    vector<string> extendedAlphabet = g.alphabet;
    extendedAlphabet.push_back("$");

    for (auto &state : document["states"].GetArray()) {
        State currState;
        currState.index = state["index"].GetInt();
        rapidjson::Value::MemberIterator M;
        for (M = state.MemberBegin(); M != state.MemberEnd(); M++) {
            string key = string(M->name.GetString());

            if (find(extendedAlphabet.begin(), extendedAlphabet.end(), key) ==
                extendedAlphabet.end()) {
                continue;
            }

            // cout << key << endl;

            for (auto &action : M->value.GetArray()) {
                Action currAction;
                currAction.actionType =
                    string(action["actionType"].GetString());
                auto type = action["actionValue"].GetType();
                if (type == Type::kStringType) {
                    currAction.actionValue =
                        stoi(action["actionValue"].GetString());
                } else {
                    currAction.actionValue = action["actionValue"].GetInt();
                }
                currState.actions[key].push_back(currAction);
            }
        }
        states.push_back(currState);
    }
    return states;
}
