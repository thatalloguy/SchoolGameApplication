//
// Created by allos on 8/22/2024.
//

#include <cstring>
#include <cstdio>
#include "string.h"

Tyche::STL::string::~string() {

    cleanUp();

}

Tyche::STL::string::string(int new_size) {
    size = new_size;
    buf = new char[size];
}

Tyche::STL::string::string(const char *buffer) {
    size = strlen(buffer);
    buf = new char[size + 1];
    strncpy_s(buf, size + 1, buffer, size);

}

Tyche::STL::string::string(const Tyche::STL::string &obj) {
    size = obj.size;
    buf = new char[size + 1];
    strncpy_s(buf, size + 1, obj.buf, size);
}


Tyche::STL::string &Tyche::STL::string::operator=(const Tyche::STL::string &obj) {
    size = obj.size;
    buf = new char[size + 1];
    strncpy_s(buf, size + 1, obj.buf, size);
    return *this;
}

Tyche::STL::string::string(Tyche::STL::string &&obj) {
    //First clean up to get rid of previous allocated memory.
    cleanUp();

    size = obj.size;

    buf = obj.buf;

    //set to null, because we now handle to buffer.
    obj.buf = nullptr;
}

void Tyche::STL::string::cleanUp() {
    if (buf != nullptr) {
        delete[] buf;
    }
}

Tyche::STL::string &Tyche::STL::string::operator=(Tyche::STL::string &&obj) {
    //First clean up to get rid of previous allocated memory.
    cleanUp();

    size = obj.size;

    buf = obj.buf;

    //set to null, because we now handle to buffer.
    obj.buf = nullptr;

    return *this;
}

Tyche::STL::string Tyche::STL::string::operator+(const Tyche::STL::string &obj) {
    string s; // new string.

    s.size = this->size + obj.size;
    s.buf = new char[s.size + 1];
    strncpy_s(s.buf, this->size + 1, this->buf, this->size); // first copy our str
    strncpy_s(s.buf + this->size, obj.size + 1, obj.buf, obj.size); // Then the other obj.

    return s;
}



bool Tyche::STL::string::operator==(const string& obj) const {
    const char* a = obj.c_str();
    const char* b = buf;
    bool result = strcmp(a, b);

    return !result;
}

bool Tyche::STL::string::operator!=(const string& obj) const {
    const char* a = obj.c_str();
    const char* b = buf;
    bool result = strcmp(a, b);

    return result;
}