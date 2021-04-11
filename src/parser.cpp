#include "parser.hpp"
#include "config.hpp"
#include "lib.hpp"
#include <iostream>
#include <string>
/** 
 * TODO
 * keep updating json
 * delimiters and keywords need to be further filtered
 * add error handling for parsing
 *
 *
 */
Parser::Parser(string &code) {

    // Loading Grammar and Parse Table
    string slr_grammar_json = fileAsString(slr_json_location);
    grammar = getGrammar(slr_grammar_json);
    states = getStates(slr_grammar_json, grammar);

    // Instantiazing Lexer
    lex = Lexer(code);

    // Setting stack with correct initial values
    st.push("$");
    st.push("0");

    // Initialize variables
    currState = 0;
}

void Parser::handleShift(string actionValue) {
    // Push Symbol and State
    if (symbol == "keyword") {
        if (std::find(datatype.begin(), datatype.end(), symbolLex) != datatype.end()) {
            //it is a datatype
            symbol = "dat";
        } else {
            symbol = symbolLex;
        }
    } else if (symbol == "delimiter") {
        symbol = symbolLex;
    }
    st.push(symbol);
    st.push(actionValue);

    // Update Current State
    currState = stoi(actionValue);

    // Fetch next symbol
    symbol = lex.getLexeme().token;
}

void Parser::handleReduce(string actionValue) {
    // Get the production rule for reduction
    Rule prod = grammar.rules[stoi(actionValue)];
    std::string nt = prod.nonterminal;
    vector<std::string> dev = prod.development;

    // Perfrom Reduction
    for (std::vector<std::string>::iterator it = dev.begin(); it != dev.end();) {
        if (!st.top().compare(*it)) {
            it++;
        }
        st.pop();
    }

    // Update Current State and Symbol
    currState = stoi(st.top());

    prevSymbol = symbol;
    prevSymbolLex = symbolLex;

    symbol = nt;
    symbolLex = "";
    // no symbolLex for a non-terminal

    // Update Stack
    st.push(nt);
}

void Parser::handleGOTO(string actionValue) {
    // Perform GOTO
    st.push(actionValue);

    // Update Symbol and Current State
    currState = stoi(actionValue);
    symbol = prevSymbol;
}

void Parser::transit(string actionType, string actionValue) {
    if (!actionType.compare("s")) {
        handleShift(actionValue);
    } else if (!actionType.compare("r")) {
        handleReduce(actionValue);
    } else if (!actionType.compare("")) {
        handleGOTO(actionValue);
    } else if (!actionType.compare("acc")) {
        symbol = "END"; // Got ACC , ending Parsing
    } else {
        throw exceptionClass::INVALID_PARSE_TABLE;
    }
}

void Parser::startParsing() {
    // Scanning first symbol
    symbol = lex.getLexeme().token;
    symbolLex = lex.getLexeme().lexeme;

    while (symbol != "END") {
        try {
            Action action = states[currState].actions[symbol][0];
            transit(action.actionType, action.actionValue);
        } catch (exceptionClass ec) {
            cout << "Invalid Parse Table! Aborting Parsing.";
            return;
        }
    }
}
