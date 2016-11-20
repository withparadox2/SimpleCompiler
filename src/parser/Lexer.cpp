//
// Created by withparadox2 on 2016/8/23.
//

#include "Lexer.h"
#include "../util/error.h"
#include "../util/layoutchars.h"

using namespace std;

Lexer::Lexer(std::string &sourceCode) : sourceCode(sourceCode), head(0), cToken(nullptr), names(Names::instance()) {
    nextChar();
}

void Lexer::nextToken() {
    bufStr.clear();
    while (true) {
        switch (ch) {
            case ' ':
            case '\t':
            case lc::FF:
                do {
                    nextChar();
                } while (ch == ' ' || ch == '\t' || ch == lc::FF);
                break;
            case '/':
                nextChar();
                if (ch == '/') {
                    //comment ranges this pos to the end of this line
                    do {
                        nextChar();
                    } while (ch != lc::CR && ch != lc::LF && ch != 0);
                } else {
                    cToken = &Token::SLASH;
                }
                return;
            case lc::CR:
                nextChar();
                if (ch == lc::LF) {
                    nextChar();
                }
                break;
            case ',':
                nextChar();
                cToken = &Token::COMMA;
                return;
            case ';':
                nextChar();
                cToken = &Token::SEMI;
                return;
            case '(':
                nextChar();
                cToken = &Token::LPAREN;
                return;
            case ')':
                nextChar();
                cToken = &Token::RPAREN;
                return;
            case '{':
                nextChar();
                cToken = &Token::LBRACE;
                return;
            case '}':
                nextChar();
                cToken = &Token::RBRACE;
                return;
            case '.':
                nextChar();
                cToken = &Token::DOT;
                return;
            case '[':
                nextChar();
                cToken = &Token::LBRACKET;
                return;
            case ']':
                nextChar();
                cToken = &Token::RBRACKET;
                return;
            case '"':
                nextChar();
                while (ch != '"' && ch != lc::CR && ch != lc::LF && ch != 0) {
                    bufStr += ch;
                    nextChar();
                }
                if (ch == '"') {
                    cToken = &Token::STRINGLITERAL;
                    nextChar();
                } else {
                    //TODO handle error
                }
                return;

            default:
                if (isIdent(ch, true)) {
                    scanIdent();
                } else if (isSpecial(ch)) {
                    scanOperator();
                } else if (ch == 0) {
                    cToken = &Token::_EOF;
                }
                return;
        }
    }
}

void Lexer::nextChar() {
    if (head >= sourceCode.size()) {
        ch = 0;
        return;
    }
    ch = sourceCode[head++];
}

Token &Lexer::token() {
    return *cToken;
}

bool Lexer::isSpecial(char c) {
    switch (c) {
        case '*':
        case '+':
        case '-':
        case '<':
        case '=':
        case '>':
            return true;
        default:
            return false;
    }
}

bool Lexer::isIdent(char c, bool isHead) {
    switch (c) {
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
        case '$':
        case '_':
            return true;
    }
    if (!isHead) {
        switch (c) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                return true;
        }
    }
    return false;
}

void Lexer::scanIdent() {
    do {
        bufStr += ch;
        nextChar();
    } while (isIdent(ch, false));

    cName = names.fromString(bufStr);
    cToken = &Token::fromName(*cName);
}

void Lexer::scanOperator() {
    while (true) {
        bufStr += ch;
        Name *newname = names.fromString(bufStr);

        Token &token = Token::fromName(*newname);
        if (token == Token::IDENTIFIER) {
            bufStr.pop_back();
            break;
        }

        cName = newname;
        cToken = &token;

        nextChar();
        if (!isSpecial(ch)) break;
    }
}

Name &Lexer::name() {
    return *cName;
}

