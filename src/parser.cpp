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

bool Parser::handleError(exceptionClass ec) {
    if (ec == exceptionClass::INVALID_SYNTAX) {
        // Emptying stack till safe symbol ';'
        while (st.top() != ";") {
            if (st.top() == "$") {
                // Safe Symbol was not found, ending parsing
                return 1;
            }
            st.pop();
        }
        st.pop(); // Pop ';'
        while (symbol != ";") {
            if (symbol == "$") {
                // Reached EOF, ending parsing
                return 1;
            }
            symbol = lex.getLexeme().token;
        }
        symbol = lex.getLexeme().token; // Start afresh from new symbol
    }
    return 0;
}

void Parser::handleShift(string actionValue) {
    // Push Symbol and State

    st.push(symbol);
    st.push(actionValue);

    // Update Current State
    currState = stoi(actionValue);

    // Fetch next symbol
    lexResult retval = lex.getLexeme(); // DAT TERM1 ;
    symbol = retval.token;
}

void Parser::handleReduce(string actionValue) {
    // Get the production rule for reduction
    Rule prod = grammar.rules[stoi(actionValue)];
    std::string nt = prod.nonterminal;
    vector<std::string> dev = prod.development;

    // Perfrom Reduction
    if (!dev.empty()) {
        for (std::vector<std::string>::reverse_iterator it = dev.rbegin(); it != dev.rend();) {
            if (!st.top().compare(*it)) {
                it++;
            }
            st.pop();
        }
    }

    // Update Current State and Symbol
    currState = stoi(st.top());

    prevSymbol = symbol;

    symbol = nt;

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
        cout << "Successfully Parsed!" << endl;
    } else {
        throw exceptionClass::INVALID_PARSE_TABLE;
    }
}

void Parser::startParsing() {
    // Scanning first symbol
    lexResult retval = lex.getLexeme();
    symbol = retval.token;

    while (symbol != "END") {
        try {
            auto found = states[currState].actions.find(symbol);
            if (found == states[currState].actions.end()) {
                throw exceptionClass::INVALID_SYNTAX;
            }
            Action action = states[currState].actions[symbol][0];
            transit(action.actionType, action.actionValue);
        } catch (exceptionClass ec) {
            if (handleError(ec)) {
                return;
            }
        }
    }
}
