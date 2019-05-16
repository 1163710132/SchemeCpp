//
// Created by chenjs on 19-5-15.
//

#ifndef SCHEMECPP_VALUES_H
#define SCHEMECPP_VALUES_H

#include <string>
#include "value.h"
#include "pch.h"

namespace scheme{

using std::string;

class SelfEvaluateValue: public Value{
public:
    bool canSelfEvaluate() override {
        return true;
    }

    Value *selfEvaluate() override {
        return this;
    }

    Value *evaluate(Context *context) override {
        return this;
    }

    void evaluate(Context* context, const Continuation& continuation) override {
        continuation(this);
    }
};

class Symbol: public Value{
public:
    string identifier;
public:
    Value* evaluate(Context *context) override;

    void evaluate(Context *context, const Continuation &continuation) override;

    bool operator<(const Symbol &rhs) const {
        return identifier < rhs.identifier;
    }

    bool operator>(const Symbol &rhs) const {
        return identifier > rhs.identifier;
    }

    bool operator<=(const Symbol &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const Symbol &rhs) const {
        return !(*this < rhs);
    }

    bool operator==(const Symbol &rhs) const {
        return identifier == rhs.identifier;
    }

    bool operator!=(const Symbol &rhs) const {
        return !(rhs == *this);
    }
};

class Pair: public Value{
public:
    Value* left;
    Value* right;

    explicit Pair(Value* left, Value* right)
        : left(left), right(right) {}

public:
    Value* evaluate(Context *context) override;

    void evaluate(Context *context, const Continuation &continuation) override;

    bool forEach(const std::function<bool (Value*, bool)> &function) override;

    Value *getLeft() override {
        return left;
    }

    Value *getRight() override {
        return right;
    }
};

class Nil: public SelfEvaluateValue{
public:
    explicit Nil() = default;
};

class Boolean: public SelfEvaluateValue{
public:
    bool booleanValue;

    explicit Boolean(bool booleanValue)
        : booleanValue(booleanValue) {}

public:
    bool asBoolean() override {
        return booleanValue;
    }

    bool isBoolean() override {
        return true;
    }
};

class Number: public SelfEvaluateValue{
public:
    bool symbol = true;
    u64 numerator = 0;
    u64 denominator = 1;
};

class Character: public SelfEvaluateValue{

};

class String: public SelfEvaluateValue{

};

class Vector: public Value{

};

class Procedure: public Value{
public:
    Value* args;
    Value* body;

    explicit Procedure(Value* args, Value* body): args(args), body(body){}
};

};

#endif //SCHEMECPP_VALUES_H
