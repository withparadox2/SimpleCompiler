#include <iostream>
#include <fstream>
#include "Token.h"
#include <vector>

using std::vector;
using std::cout;
using std::string;

void eatLine(vector<string> &tokens, string &);

int main() {
    string filePath("E:/programming/compiler/SimpleCompiler/TestHelloWorld/HelloWorld.java");
    std::ifstream is(filePath.c_str());
    vector<string> tokens;
    if (is.is_open()) {
        string line;
        while (std::getline(is, line)) {
            eatLine(tokens, line);
        }
        is.close();
    }

    for (int i = 0 ; i < tokens.size(); i++) {
        std::cout << "token[" << i << "] = " << tokens[i] << std::endl;
    }

    return 0;
}

void push(vector<string> &vec, char *line, char *&w_start) {
    char c = *line;
    *line = 0;
    vec.push_back(w_start);
    w_start = nullptr;
    *line = c;
}

void eatLine(vector<string> &tokens, string &inputLine) {
    char *line = const_cast<char *>(inputLine.c_str());

    char *t_start = nullptr;
    while (char c = *line) {
        switch (c) {
            case ' ':
                if (t_start != nullptr) {
                    push(tokens, line, t_start);
                }
                break;
            case '/':
                if (*(line + 1) == '/') {
                    *(line + 1) = 0;
                }
                break;
            case '(':
            case ')':
            case '{':
            case '}':
            case '.':
                if (t_start != nullptr) {
                    push(tokens, line, t_start);
                }
                t_start = line;
                push(tokens, line + 1, t_start);
                break;
            case '"':
                if (t_start == nullptr) {
                    t_start = line;
                }
                //不要陷入死循环
                while (*(++line) != '"' && *line != 0) {}
                push(tokens, 1 + line, t_start);
                break;
            default:
                if (t_start == nullptr) {
                    t_start = line;
                }
                break;
        }
        line++;
    }

}


