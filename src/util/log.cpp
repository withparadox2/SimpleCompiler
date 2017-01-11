//
// Created by withparadox2 on 2017/1/11.
//
#include "log.h"

void log(string tag, string info) {
    cout << tag << ":" << info << endl;
}

void log(string info) {
    cout << info << endl;
}