//
// Created by withparadox2 on 2016/8/23.
//

#include "Lexer.h"
#include <iostream>

using namespace std;

Lexer::Lexer(ifstream &stream) : stream(stream), head(0), sourceCode("") {}

Token Lexer::buildToken(string &text, int state) {
    int id = Token::T_END;
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
    }
    return Token(id, text);
}

//can handle HelloWorld only
Token Lexer::nextToken() {
    int state = STATE_START;
    string text = "";
    while (char c = nextChar()) {
        if (state == STATE_COMMENT) {
            if (c != '\n') {
                continue;
            }
        }
        if (state == STATE_STR) {
            if (c != '"') {
                text += c;
                continue;
            }
        }
        switch (c) {
            case ' ':
                if (state != STATE_START) {
                    return buildToken(text, state);
                }
                break;
            case '/':
                if (state != STATE_START && state != STATE_STR) {
                    return buildToken(text, state);
                }
                if (state != STATE_STR) {
                    if (nextChar() != '/') {
                        cout << "wrong comment" << endl;
                        exit(-1);
                    } else {
                        state = STATE_COMMENT;
                    }
                }
                break;
            case '\n':
                if (state != STATE_COMMENT && state != STATE_START) {
                    return buildToken(text, state);
                }
                state = STATE_START;
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
                if (state != STATE_STR) {
                    state = STATE_STR;
                } else {
                    return buildToken(text, state);
                }
                break;
            default:
                state = STATE_ID;
                text += c;
                break;
        }
    }
    string empty = "";
    return Token(-1, empty);
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
    int size = sourceCode.size();
    if (head >= size) {
        return 0;
    }

    return sourceCode[head++];
}
