#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "Parser.h"
#include "Common.h"

using namespace std;

int main() {
    string filePath(FILE_PATH);
    std::ifstream is(filePath.c_str());

    if (is.is_open()) {
        Lexer lexer(is);
        Parser parser(lexer);
        parser.parse();
        is.close();
    }
    return 0;
}




