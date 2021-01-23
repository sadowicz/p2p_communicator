#pragma once

#include <string>

using namespace std;

class strbuilder {

public:
    strbuilder() {}
    strbuilder(string& first) {
        this->internal = string(first);
    }

    strbuilder(char* first) {
        this->internal = string(first);
    }

    strbuilder(const char* first) {
        this->internal = string(first);
    }

    string& get() {
        return internal;
    }

    strbuilder& add(string& next) {
        internal.append(next);
        return *this;
    }

    strbuilder& add(char* next) {
        internal.append(string(next));
        return *this;
    }

    strbuilder& add(const char* next) {
        internal.append(string(next));
        return *this;
    }

    strbuilder& operator+(string& next) {
        internal.append(next);
        return *this;
    }

    strbuilder& operator+(char* next) {
        internal.append(string(next));
        return *this;
    }

    strbuilder& operator+(const char* next) {
        internal.append(string(next));
        return *this;
    }

private:
    string internal;

};
