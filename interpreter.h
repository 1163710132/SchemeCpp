//
// Created by chenjs on 19-5-15.
//

#ifndef SCHEMECPP_INTERPRETER_H
#define SCHEMECPP_INTERPRETER_H

#include <string>
#include "context.h"
#include "value.h"

namespace scheme{

class Interpreter{
public:
    Context* global;

    explicit Interpreter(){
        global = new Context(nullptr);
    }

    Value* interpret(Value* input){
        return input->evaluate(global);
    }
};

};

#endif //SCHEMECPP_INTERPRETER_H
