//
// Created by chenjs on 19-5-15.
//

#ifndef SCHEMECPP_CONTEXT_H
#define SCHEMECPP_CONTEXT_H

#include <map>
#include "value.h"
#include "values.h"

namespace scheme{

class Context{
private:
    Context* parent;
    std::map<Symbol, Value*> localValues;

public:

    explicit Context(Context* parent): parent(parent){};
public:
    void defineValue(const Symbol& symbol, Value* value){
        localValues.insert({symbol, value});
    }

    bool setValue(const Symbol& symbol, Value* newValue){
        if(localValues.count(symbol) > 0){
            localValues.at(symbol) = newValue;
            return true;
        }else{
            if(parent != nullptr){
                return parent->setValue(symbol, newValue);
            }else{
                return false;
            }
        }
    }

    Value* getValue(const Symbol& symbol){
        if(localValues.count(symbol) > 0){
            return localValues.at(symbol);
        }else{
            return nullptr;
        }
    }

    Context* fork(){
        auto* child = new Context(this);
        return child;
    }
};

}

#endif //SCHEMECPP_CONTEXT_H
