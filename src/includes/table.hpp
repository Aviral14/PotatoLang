#pragma once

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "types.hpp"
#include <algorithm>

using namespace rapidjson;

Grammar getGrammar(string jsonString);
vector<State> getStates(string jsonString, Grammar g);