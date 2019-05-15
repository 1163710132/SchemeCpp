//
// Created by chenjs on 19-5-15.
//

#ifndef SCHEMECPP_VALUE_H
#define SCHEMECPP_VALUE_H

#include "context.h"

class Value;

class Value{
public:
    virtual bool isBoolean(){
        return false;
    }
    virtual bool asBoolean(){
        return true;
    }
    virtual bool isNumber(){
        return false;
    }
    virtual bool isComplex(){
        return false;
    }
    virtual bool isPair(){
        return false;
    }
    virtual bool isNil(){
        return false;
    }
    virtual bool isString(){
        return false;
    }
    virtual bool isVector(){
        return false;
    }
    virtual bool isList(){
        return false;
    }
    virtual Value* getListElementAt(int index){
        return nullptr;
    }
    virtual Value* getListElementRange(int from){
        return nullptr;
    }
    virtual Value* getLeft(){
        return nullptr;
    }
    virtual Value* getRight(){
        return nullptr;
    }
    virtual int getListLength(){
        return 0;
    }
    virtual bool isProcedure(){
        return false;
    }
    virtual bool isSymbol(){
        return false;
    }
    virtual Value* callProcedure(Value* args){
        return nullptr;
    }
    virtual bool canSelfEvaluate(){
        return false;
    }
    virtual Value* selfEvaluate(){
        return nullptr;
    }
    virtual Value* evaluate(Context* context){
        return nullptr;
    }
    template <typename T>
    T* as(){
        return static_cast<T>(this);
    }
};

#endif //SCHEMECPP_VALUE_H
