//
// Created by withparadox2 on 2017/1/11.
//

#ifndef SIMPLECOMPILER_LOG_H
#define SIMPLECOMPILER_LOG_H

#include <string>
#include <iostream>

using namespace std;

//TODO We need a robust, flexible, amazing logger, not a toy :-)
void log(string tag, string info);
void log(string info);

#endif //SIMPLECOMPILER_LOG_H
