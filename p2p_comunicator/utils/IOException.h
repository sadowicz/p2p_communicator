#ifndef IO_EXCEPTION_H
#define IO_EXCEPTION_H

#include <exception>
#include <stdexcept>
#include <string>

using namespace std;

class IOException : public logic_error {

public:
    explicit IOException(const char* message)
            : logic_error(message) {}

    explicit IOException(const std::string& message)
            : logic_error(message) {}

    virtual ~IOException() noexcept {}

};

#endif // IO_EXCEPTION_H
