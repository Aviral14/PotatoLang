#include "lib.hpp"

string fileAsString(string fileLocation) {
    std::ifstream inFile;
    inFile.open(fileLocation); // open the input file

    std::stringstream strStream;
    strStream << inFile.rdbuf();       // read the file
    std::string str = strStream.str(); // str holds the content of the file
    return str;
}