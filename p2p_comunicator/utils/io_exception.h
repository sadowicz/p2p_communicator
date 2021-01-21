#ifndef IO_EXCEPTION_H
#define IO_EXCEPTION_H

#include <exception>
#include <stdexcept>
#include <string>

using namespace std;

class io_exception : public logic_error {

public:
    explicit io_exception(const char* message)
            : msg_(message) {}

    explicit io_exception(const std::string& message)
            : msg_(message) {}

    virtual ~io_exception() noexcept {}

    virtual const char* what() const noexcept {
       return msg_.c_str();
    }

protected:
    std::string msg_;
};

#endif // IO_EXCEPTION_H
