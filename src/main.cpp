#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "Parser.h"
#include "common.h"
#include "Code.h"

using namespace std;

int main() {
    string filePath(FILE_PATH);
    std::ifstream is(filePath.c_str());
    if (is.is_open()) {
        Lexer lexer(is);
        Parser parser(lexer);
        ClassNode* node = parser.parse();
        is.close();
        Code code(node, FILE_PATH_ROOT "hello.class");
        code.start();
    }
    return 0;
}




