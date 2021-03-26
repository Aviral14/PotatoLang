#include "parser.hpp"

void testParser() {
    string slr_grammar_json = fileAsString(slr_json_location);
    Grammar grammar = getGrammar(slr_grammar_json);
    vector<State> states = getStates(slr_grammar_json, grammar);
    std::cout << "Parse table generation working OK" << endl;
}