//
// Created by withparadox2 on 2016/8/23.
//

#include "Lexer.h"
#include "error.h"

using namespace std;

Lexer::Lexer(ifstream &stream) : stream(stream), head(0) {}

Token Lexer::buildToken(string &text, int state) {
    int id;
    switch (state) {
        case STATE_CHAR:
            id = Token::T_CHAR;
            break;
        case STATE_STR:;
            id = Token::T_STR;
            break;
        case STATE_ID:
            id = Token::T_ID;
            break;
        default:
            id = Token::T_END;
    }
    return Token(id, text);
}

//can handle HelloWorld only
Token Lexer::nextToken() {
    int state = STATE_START;
    string text = "";
    while (char c = nextChar()) {
        switch (c) {
            case ' ':
                if (state != STATE_START) {
                    return buildToken(text, state);
                }
                break;
            case '/':
                if (state != STATE_START) {
                    return buildToken(text, state);
                }
                if (nextChar() != '/') {
                    error("wrong comment");
                }
                while (nextChar() != '\n') {}
                break;
            case '\n':
                if (state != STATE_START) {
                    return buildToken(text, state);
                }
                break;
            case '(':
            case ')':
            case '{':
            case '}':
            case '.':
            case '[':
            case ']':
                if (state != STATE_START) {
                    rollBack();
                    return buildToken(text, state);
                }
                text += c;
                return buildToken(text, STATE_CHAR);
            case '"':
                if (state != STATE_START) {
                    return buildToken(text, state);
                }

                while ((c = nextChar()) != '"') {
                    if (c == '\n') {
                        error("string can only appear in single line");
                    }
                    text += c;
                }
                return buildToken(text, STATE_STR);
            default:
                state = STATE_ID;
                text += c;
                break;
        }
    }
    string empty = "";
    return Token(Token::T_END, empty);
}

void Lexer::rollBack() {
    head--;
}

char Lexer::nextChar() {
    if (sourceCode.size() == 0 || head >= sourceCode.size()) {
        string line;
        if (getline(stream, line)) {
            sourceCode += line;
            sourceCode += '\n';
        }
    }
    if (head >= sourceCode.size()) {
        return 0;
    }
    return sourceCode[head++];
}

