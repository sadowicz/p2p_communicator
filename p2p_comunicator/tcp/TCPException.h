#pragma once

#include <exception>
#include <stdexcept>
#include <string>

using namespace std;

class TCPException : public logic_error {

public:
    explicit TCPException(const char* message)
            : logic_error(message) {}

    explicit TCPException(const std::string& message)
            : logic_error(message) {}

    virtual ~TCPException() noexcept {}

};