#pragma once

#include <string>

using namespace std;

class strbuilder {

public:
    strbuilder() {}

    class end {
    };

    string operator+(strbuilder::end _) {
        return internal;
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
