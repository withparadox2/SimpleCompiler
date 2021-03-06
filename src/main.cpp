#include <iostream>
#include <fstream>
#include <memory>
#include <iterator>
#include "parser/Lexer.h"
#include "parser/Parser.h"
#include "util/layoutchars.h"
#include "tree/pretty.h"
#include "comp/enter.h"
#include "main.h"
#include "comp/attr.h"
#include "jvm/gen.h"
#include "jvm/ClassWriter.h"

using namespace std;

string* readByPath(const string& filePath) {
    ifstream is(filePath.c_str());
    string* source = nullptr;
    if(is.is_open()) {
        source = new string(istreambuf_iterator<char>(is),
                            istreambuf_iterator<char>());
        is.close();
    }
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

    Env<AttrContext>* env = Enter::instance().typeEnvs.at(clazz->sym).get();
    Attr::instance().attrib(env);
    Gen::instance().genClass(env, clazz.get());

    ClassWriter writer;
    writer.writeClass(clazz.get(), FILE_PATH_ROOT "Computer.class");
    return 0;
}