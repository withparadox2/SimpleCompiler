#include <iostream>
#include <fstream>
#include <memory>
#include "parser/Lexer.h"
#include "parser/Parser.h"
#include "util/layoutchars.h"
#include "tree/pretty.h"
#include "comp/enter.h"
#include "main.h"
#include "comp/attr.h"
#include "jvm/gen.h"

using namespace std;

//Break line at the end of each line.
void readFileContent(ifstream& is, string& result) {
    if (is.is_open()) {
        string line;
        while (getline(is, line)) {
            result += line;
            result += lc::CR;
        }
        is.close();
    }
}

string* readByPath(const string& filePath) {
    string* source = new string;
    ifstream is(filePath.c_str());
    readFileContent(is, *source);
    return source;
}

int main() {
    string filePath(FILE_PATH);
    std::unique_ptr<string> strPtr(readByPath(filePath));
    Lexer lexer(*strPtr);
    Parser parser(lexer);

    JCClassDecl::Ptr clazz(parser.buildClass());
    Pretty p;
    clazz->accept(&p);

    Enter::instance().complete(clazz.get(), nullptr);

    Env* env = Enter::instance().typeEnvs.at(clazz->sym);
    Attr::instance().attrib(env);
    Gen::instance().genClass(env, clazz.get());
    return 0;
}