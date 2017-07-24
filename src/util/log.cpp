//
// Created by withparadox2 on 2017/1/11.
//
#include <cstdarg>
#include <vector>
#include <cassert>
#include "log.h"

#define ENABLE_LOG

void log(string tag, string info) {
#ifdef ENABLE_LOG
    cout << tag << ":" << info << endl;
#endif
}

void log(string info) {
#ifdef ENABLE_LOG
    cout << info << endl;
#endif
}

void log(const char* format, ...) {
#ifdef ENABLE_LOG
    std::vector<char> buf;

    va_list vlist;
    va_start(vlist, format);
    int l = vsnprintf(&buf.front(), buf.capacity(), format, vlist);
    va_end(vlist);

    if (l > buf.capacity()) {
        buf.reserve(l + 1);
        va_start(vlist, format);
        l = vsnprintf(&buf.front(), buf.capacity(), format, vlist);
        va_end(vlist);
    }

    assert(l >= 0);
    assert(l < buf.capacity());

    cout << &buf.front() << endl;
#endif
}
