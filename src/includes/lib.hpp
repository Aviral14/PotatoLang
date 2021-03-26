
#pragma once
#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream
#include <streambuf>
#include <string>

using namespace std;

/**
 * Read a file as a string.
 *
 * @param fileLocation location of the file to be read.
 *
 **/
string fileAsString(string fileLocation);