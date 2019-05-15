//
// Created by chenjs on 19-5-16.
//

#ifndef SCHEMECPP_PARSER_H
#define SCHEMECPP_PARSER_H

#include <vector>
#include "value.h"
#include "token.h"

namespace scheme{

class Parser{
public:
    std::vector<Value*> parse(const std::vector<Token*>& tokens){
        std::vector<Value*> stack;

        for(auto* token: tokens){
            switch (token->type){
                case TokenType::String:{

                }
                case TokenType::Identifier:{

                }
                case TokenType::Boolean:{

                }
                case TokenType::Number:{

                }
                case TokenType::Character:{

                }
                case TokenType::Special:{

                }
            }
        }
    }
};

}

#endif //SCHEMECPP_PARSER_H
