//
// Created by chenjs on 19-5-15.
//

#ifndef SCHEMECPP_LEXER_H
#define SCHEMECPP_LEXER_H

#include <vector>
#include <string>
#include <string_view>
#include <regex>
#include <map>

namespace scheme{

class Matcher{
public:
    virtual Token* match(std::string_view view){
        return nullptr;
    }
};

class Lexer{
public:
    std::map<TokenType, Matcher*> tokenMatchers;

    std::vector<Token*> doLex(const std::string& input){
        size_t pos = 0;
        std::vector<Token*> tokens;
        while (true){
            Token* token = nullptr;
            for(auto& pair: tokenMatchers){
                auto* match = pair.second->match(std::string_view(input).substr(pos));
                if(match != nullptr){
                    token = match;
                }
            }
            if(token == nullptr){
                break;
            }else{
                tokens.push_back(token);
            }
        }
        return tokens;
    }
};
}

#endif //SCHEMECPP_LEXER_H
