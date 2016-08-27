#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "Parser.h"

using namespace std;

int main() {
    string filePath("E:/programming/compiler/SimpleCompiler/TestHelloWorld/HelloWorld.java");
    std::ifstream is(filePath.c_str());

    if (is.is_open()) {
        Lexer lexer(is);
        Parser parser(lexer);
        parser.parse();
        is.close();
    }
    return 0;
}




