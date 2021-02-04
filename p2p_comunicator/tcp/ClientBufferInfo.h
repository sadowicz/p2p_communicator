#pragma once

#include <string>

class ClientBufferInfo {

public:
    ClientBufferInfo() : address(""), buffer(new std::string()), expectedSize(0) {}
    ClientBufferInfo(std::string address);

    void setExpectedSize(size_t expectedSize);
    bool isDone();
    size_t getExpectedSize();
    std::string* getBuffer();
    std::string& getAddress();
    void add(char* str);

private:
    std::string address;
    std::string* buffer;
    size_t expectedSize;
};
