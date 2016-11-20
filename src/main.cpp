#include <iostream>
#include <fstream>
#include "parser/Lexer.h"
#include "parser/Parser.h"
#include "common.h"
#include "Code.h"
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

void testLexer(Lexer &lexer) {
    lexer.nextToken();
    Token &t1 = lexer.token();
    while (lexer.token() != Token::_EOF) {
        Token &t = lexer.token();
        if (t.desc().length() == 0) {
            cout << lexer.bufStr;
        } else {
            cout << t.desc() << endl;
        }
        lexer.nextToken();
    }
}

int main() {
    string filePath(FILE_PATH);
    std::ifstream is(filePath.c_str());
    std::string sourceCode;
    readFileContent(is, sourceCode);

    Lexer lexer(sourceCode);

    testLexer(lexer);

//    Parser parser(lexer);
//    ClassNode *node = parser.parse();
//    is.close();
//    Code code(node, FILE_PATH_ROOT "HelloWorld.class");
//    code.start();
    return 0;
}






