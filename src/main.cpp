#include <iostream>
#include <fstream>
#include "parser/Lexer.h"
#include "parser/Parser.h"
#include "common.h"
#include "util/layoutchars.h"

using namespace std;

void readFileContent(std::ifstream &is, std::string &result) {
    if (is.is_open()) {
        string line;
        while (getline(is, line)) {
            result += line;
            result += lc::CR;
        }
        is.close();
    }
}

int main() {
    //TODO Manage file IO
    string filePath(FILE_PATH);
    std::ifstream is(filePath.c_str());
    std::string sourceCode;
    readFileContent(is, sourceCode);

    Lexer lexer(sourceCode);
    Parser parser(lexer);
    parser.buildClass();

//    Parser parser(lexer);
//    ClassNode *node = parser.parse();
//    is.close();
//    Code code(node, FILE_PATH_ROOT "HelloWorld.class");
//    code.start();
    return 0;
}






