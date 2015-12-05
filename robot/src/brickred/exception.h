#ifndef BRICKRED_EXCEPTION_H
#define BRICKRED_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace brickred {

class SystemErrorException : public std::runtime_error {
public:
    SystemErrorException(const std::string &what_arg) :
        std::runtime_error(what_arg) {}
};

} // namespace brickred

#endif
