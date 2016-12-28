#include <iostream>
#include <fstream>
#include "parser/Lexer.h"
#include "parser/Parser.h"
#include "common.h"
#include "util/layoutchars.h"
#include "tree/pretty.h"

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
    string filePath(FILE_PATH);
    std::ifstream is(filePath.c_str());
    std::string sourceCode;
    readFileContent(is, sourceCode);

    Lexer lexer(sourceCode);
    Parser parser(lexer);
    JCClassDecl *clazz = parser.buildClass();

//    Pretty p;
//    clazz->accept(p);

    return 0;
}






