//
// Created by chenjs on 19-5-16.
//

#ifndef SCHEMECPP_TOKEN_H
#define SCHEMECPP_TOKEN_H

#include <string>

namespace scheme{

enum class TokenType{
    Identifier,
    Boolean,
    Number,
    Character,
    String,
    Special
};

struct Token{
    std::string raw;
    TokenType type;

    explicit Token(std::string raw, TokenType type)
            : raw(std::move(raw)), type(type){}
};
}

#endif //SCHEMECPP_TOKEN_H
