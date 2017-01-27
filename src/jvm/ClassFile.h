//
// Created by withparadox2 on 2017/1/27.
//

#ifndef SIMPLECOMPILER_CLASSFILE_H
#define SIMPLECOMPILER_CLASSFILE_H


class ClassFile {
public:
    static const int JAVA_MAGIC = 0xCAFEBABE;
    // see Target
    static const int CONSTANT_Utf8 = 1;
    static const int CONSTANT_Unicode = 2;
    static const int CONSTANT_Integer = 3;
    static const int CONSTANT_Float = 4;
    static const int CONSTANT_Long = 5;
    static const int CONSTANT_Double = 6;
    static const int CONSTANT_Class = 7;
    static const int CONSTANT_String = 8;
    static const int CONSTANT_Fieldref = 9;
    static const int CONSTANT_Methodref = 10;
    static const int CONSTANT_InterfaceMethodref = 11;
    static const int CONSTANT_NameandType = 12;
    static const int CONSTANT_MethodHandle = 15;
    static const int CONSTANT_MethodType = 16;
    static const int CONSTANT_InvokeDynamic = 18;

    static const int MAX_PARAMETERS = 0xff;
    static const int MAX_DIMENSIONS = 0xff;
    static const int MAX_CODE = 0xffff;
    static const int MAX_LOCALS = 0xffff;
    static const int MAX_STACK = 0xffff;
};


#endif //SIMPLECOMPILER_CLASSFILE_H
