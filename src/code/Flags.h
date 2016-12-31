//
// Created by withparadox2 on 2016/11/27.
//

#ifndef SIMPLECOMPILER_FLAGS_H
#define SIMPLECOMPILER_FLAGS_H

#include <cstdint>

class Flags {
public:
    static const int PUBLIC = 1 << 0;
    static const int PRIVATE = 1 << 1;
    static const int PROTECTED = 1 << 2;
    static const int STATIC = 1 << 3;
    static const int FINAL = 1 << 4;
    static const int SYNCHRONIZED = 1 << 5;
    static const int VOLATILE = 1 << 6;
    static const int TRANSIENT = 1 << 7;
    static const int NATIVE = 1 << 8;
    static const int INTERFACE = 1 << 9;
    static const int ABSTRACT = 1 << 10;
    static const int STRICTFP = 1 << 11;

    /** Flag is set for a variable symbol if the variable's definition
    *  has an initializer part.
    */
    static const int HASINIT = 1 << 18;
    /** Flag for class symbols is set and later re-set to indicate that a class
     *  has been entered but has not yet been attributed.
     */
    static const int UNATTRIBUTED = 1 << 28;

    /** Flag that marks a generated default constructor.
     */
    static const uint64_t GENERATEDCONSTR = (uint64_t)1 << 36;

    static const int AccessFlags = PUBLIC | PROTECTED | PRIVATE;
};


#endif //SIMPLECOMPILER_FLAGS_H
