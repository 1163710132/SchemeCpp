//
// Created by chenjs on 19-5-15.
//

#include "values.h"
#include "exception.h"

#include <vector>
#include "context.h"

namespace scheme{

Value* Pair::evaluate(Context *context) {
    Value* left = this->left->evaluate(context);
    if(left == nullptr && this->left->isSymbol()) {
        //TODO evil
        auto *symbol = this->left->as<Symbol>();
        if (symbol->identifier == string("define")) {
            //case define
            Value *pattern = this->right->getListElementAt(0);
            Value *body = this->right->getListElementRange(1);
            if (pattern->isSymbol()) {
                //case common define
                Value *evaluatedRight = body->evaluate(context);
                context->defineValue(*pattern->as<Symbol>(), evaluatedRight);
                return new Nil();
            } else if (pattern->isList()) {
                //case function define
                Value *functionName = pattern->getListElementAt(0);
                Value *functionArgs = pattern->getListElementRange(1);
                context->defineValue(*functionName->as<Symbol>(), new Procedure(functionArgs, body));
                return new Nil();
            } else throw SchemeException("Argument error");
        } else if (symbol->identifier == string("let")) {
            //case let
            Value *localDefines = this->right->getListElementAt(0);
            while (!localDefines->isNil()) {
                Value *localDefine = localDefines->getListElementAt(0);
                localDefines = localDefines->getListElementRange(1);
                Value *localName = localDefine->getLeft();
                Value *localBody = localDefine->getRight()->getLeft();
                Value *localValue = localBody->evaluate(context);
                context->defineValue(*localName->as<Symbol>(), localValue);
            }
            Value *letBody = this->right->getListElementAt(1);
            return letBody->evaluate(context);
        } else if (symbol->identifier == string("quote")) {
            //case quote
            Value *quoteBody = this->right->getListElementAt(0);
            return quoteBody;
        } else if (symbol->identifier == string("lambda")) {
            //case lambda
            Value *lambdaArgs = this->right->getListElementAt(0);
            Value *lambdaBody = this->right->getListElementAt(1);
            return new Procedure(lambdaArgs, lambdaBody);
        } else if (symbol->identifier == string("if")) {
            //case if
            Value *testBody = this->right->getListElementAt(0);
            Value *thenBody = this->right->getListElementAt(1);
            Value *elseBody = this->right->getListElementAt(2);
            bool test = testBody->evaluate(context)->asBoolean();
            if (test) {
                return thenBody->evaluate(context);
            } else {
                return elseBody->evaluate(context);
            }
        } else if (symbol->identifier == string("set!")) {
            //case set!
            Value *variableName = this->right->getListElementAt(0);
            Value *variableBody = this->right->getListElementAt(1);
            Value *variableValue = variableBody->evaluate(context);
            context->setValue(*variableName->as<Symbol>(), variableValue);
            return new Nil();
        } else if (symbol->identifier == string("begin")) {
            //case begin
            Value *lastResult = new Nil();
            Value *remaining = this->right;
            while (!remaining->isNil()) {
                Value *currentBody = remaining->getLeft();
                remaining = remaining->getRight();
                lastResult = currentBody->evaluate(context);
            }
            return lastResult;
        } else if (symbol->identifier == string("cond")) {
            //case cond
            Value *conditions = this->right;
            while (!conditions->isNil()) {
                Value *currentCondition = conditions->getLeft();
                conditions = conditions->getRight();
                Value *test = currentCondition->getLeft();
                Value *then = currentCondition->getRight()->getLeft();
                if (test->evaluate(context)->asBoolean()) {
                    return then->evaluate(context);
                }
            }
            return new Nil();
        } else if (symbol->identifier == string("and")) {
            //case and
            Value *tests = this->right;
            Value *lastResult = new Boolean(true);
            while (!tests->isNil()) {
                Value *currentTest = tests->getLeft();
                lastResult = currentTest->evaluate(context);
                if (!lastResult->asBoolean()) {
                    return lastResult;
                }
            }
            return lastResult;
        } else if (symbol->identifier == string("or")) {
            //case or
            Value *tests = this->right;
            Value *lastResult = new Boolean(false);
            while (!tests->isNil()) {
                Value *currentTest = tests->getLeft();
                lastResult = currentTest->evaluate(context);
                if (lastResult->asBoolean()) {
                    return lastResult;
                }
            }
            return lastResult;
        } else return new Nil();
        //TODO
        //case do
        //case delay
    }
    return left;
}

};
