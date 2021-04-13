#pragma once
#include <string>
#include <unordered_map>
#include <vector>
using std::string;
using std::unordered_map;
using std::vector;

/*! \struct LexResult
    \brief Store the result of lexer as a pair
    Structure for storing token, lexeme pairs from lexer
*/
struct lexResult {
    string token;
    string lexeme;
    int line;
};

/*! \enum characterClass
    \brief Character Classes
    invalid character- all characters which are not specified in Regular Grammar
    alpha- a-z|A-Z|_
    zero- 0
    numeric- 1-9
    whitespace- ' '|'\n'|'\t'
    endoffile- $
 */
enum class characterClass {
    INVALID = -1,
    ALPHA = 0,
    ZERO,
    NUMERIC,
    DOT,
    WHITESPACE,
    ARITH,
    LOGIC,
    EQU,
    DELIM,
    QUOTE,
    AND,
    OR,
    ENDOFFILE,

};

enum class exceptionClass {
    BAD_TOKEN,
    BAD_ESCAPE_SEQUENCE,
    BAD_CHARACTER,
    BAD_OPERATOR,
    BAD_TERMINATOR,
    UNTERMINATED_COMMENT_BLOCK,
    INVALID_PARSE_TABLE,
};

struct Rule {
    size_t index;
    string nonterminal;
    vector<string> pattern;
    vector<string> development;
};

struct Action {
    string actionType;
    string actionValue;
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