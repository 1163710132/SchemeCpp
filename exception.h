//
// Created by chenjs on 19-5-15.
//

#ifndef SCHEMECPP_EXCEPTION_H
#define SCHEMECPP_EXCEPTION_H

#include <string>
#include <exception>

using std::exception;
using std::string;

namespace scheme{

class SchemeException: exception{
private:
    std::string reason;
public:
    explicit SchemeException(string reason): reason(std::move(reason)){}

    const char *what() const noexcept override {
        return reason.c_str();
    }
};

};

#endif //SCHEMECPP_EXCEPTION_H
