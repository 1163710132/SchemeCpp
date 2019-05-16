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

static void evaluateCond(Value* conditions, Context* context, const Continuation& continuation){
    if(!conditions->isNil()){
        Value* condition = conditions->getLeft();
        Value* remaining = conditions->getRight();
        Value* test = condition->getLeft();
        Value* then = condition->getRight()->getLeft();
        test->evaluate(context, [then, remaining, context, continuation](Value* testResult)->void{
            if(testResult->asBoolean()){
                then->evaluate(context, continuation);
            }else{
                evaluateCond(remaining, context, continuation);
            }
        });
    }else{
        continuation(new Nil());
    }
}

static void evaluateAnd(Value* conditions, Context* context, const Continuation& continuation){
    if(!conditions->isNil()){
        Value* test = conditions->getLeft();
        Value* remaining = conditions->getRight();
        if(remaining->isNil()){
            test->evaluate(context, continuation);
        }else{
            test->evaluate(context, [remaining, context, continuation](Value* testResult)->void{
                if(testResult->asBoolean()){
                    evaluateAnd(remaining, context, continuation);
                }else{
                    continuation(new Boolean(false));
                }
            });
        }
    }else{
        continuation(new Boolean(true));
    }
}

static void evaluateOr(Value* conditions, Context* context, const Continuation& continuation){
    if(!conditions->isNil()){
        Value* test = conditions->getLeft();
        Value* remaining = conditions->getRight();
        if(remaining->isNil()){
            test->evaluate(context, continuation);
        }else{
            test->evaluate(context, [remaining, context, continuation](Value* testResult)->void{
                if(!testResult->asBoolean()){
                    evaluateOr(remaining, context, continuation);
                }else{
                    continuation(new Boolean(true));
                }
            });
        }
    }else{
        continuation(new Boolean(false));
    }
}

void Pair::evaluate(Context* context, const Continuation& continuation) {
    this->left->evaluate(context, [this, context, continuation](auto* evaluatedLeft)->void{
        if(evaluatedLeft == nullptr && this->left->isSymbol()) {
            //TODO evil
            auto *symbol = this->left->as<Symbol>();
            if (symbol->identifier == string("define")) {
                //case define
                Value *pattern = this->right->getListElementAt(0);
                Value *body = this->right->getListElementRange(1);
                if (pattern->isSymbol()) {
                    //case common define
                    body->evaluate(context, [context, pattern, continuation](auto* evaluatedBody)->void{
                        context->defineValue(*pattern->as<Symbol>(), evaluatedBody);
                        continuation(new Nil());
                    });
                } else if (pattern->isList()) {
                    //case function define
                    Value *functionName = pattern->getListElementAt(0);
                    Value *functionArgs = pattern->getListElementRange(1);
                    context->defineValue(*functionName->as<Symbol>(), new Procedure(functionArgs, body));
                    continuation(new Nil());
                } else throw SchemeException("Argument error");
            } else if (symbol->identifier == string("let")) {
                //case let
                Value *localDefines = this->right->getListElementAt(0);
                localDefines->forEach([context](auto* localDefine, bool hasMore)->bool{
                    Value *localName = localDefine->getLeft();
                    Value *localBody = localDefine->getRight()->getLeft();
                    localBody->evaluate(context, [context, localName](auto* localValue)->void{
                        context->defineValue(*localName->as<Symbol>(), localValue);
                    });
                    return true;
                });
                Value *letBody = this->right->getListElementAt(1);
                letBody->evaluate(context, continuation);
            } else if (symbol->identifier == string("quote")) {
                //case quote
                Value *quoteBody = this->right->getListElementAt(0);
                continuation(quoteBody);
            } else if (symbol->identifier == string("lambda")) {
                //case lambda
                Value *lambdaArgs = this->right->getListElementAt(0);
                Value *lambdaBody = this->right->getListElementAt(1);
                Value* procedure = new Procedure(lambdaArgs, lambdaBody);
                continuation(procedure);
            } else if (symbol->identifier == string("if")) {
                //case if
                Value *testBody = this->right->getListElementAt(0);
                Value *thenBody = this->right->getListElementAt(1);
                Value *elseBody = this->right->getListElementAt(2);
                testBody->evaluate(context, [context, continuation, thenBody, elseBody](auto* testValue)->void{
                    bool test = testValue->asBoolean();
                    if(test){
                        thenBody->evaluate(context, continuation);
                    }else{
                        elseBody->evaluate(context, continuation);
                    }
                });
            } else if (symbol->identifier == string("set!")) {
                //case set!
                Value *variableName = this->right->getListElementAt(0);
                Value *variableBody = this->right->getListElementAt(1);
                variableBody->evaluate(context, [continuation, context, variableName](auto* variableValue)->void{
                    context->setValue(*variableName->as<Symbol>(), variableValue);
                    continuation(new Nil());
                });
            } else if (symbol->identifier == string("begin")) {
                //case begin
                Value *lastResult = new Nil();
                Value *remaining = this->right;
                Value* expressions = this->right;
                if(expressions->isNil()){
                    continuation(new Nil());
                }else{
                    expressions->forEach([context, continuation](Value* expression, bool hasMore)->bool{
                        if(hasMore){
                            expression->evaluate(context, [](auto* expressionValue)->void{});
                        }else{
                            expression->evaluate(context, continuation);
                        }
                        return true;
                    });
                }
            } else if (symbol->identifier == string("cond")) {
                //case cond
                Value *conditions = this->right;
                evaluateCond(conditions, context, continuation);
            } else if (symbol->identifier == string("and")) {
                //case and
                Value* conditions = this->right;
                evaluateAnd(conditions, context, continuation);
            } else if (symbol->identifier == string("or")) {
                //case or
                Value* conditions = this->right;
                evaluateOr(conditions, context, continuation);
            } else continuation(new Nil());
            //TODO
            //case do
            //case delay
        }
    });
}

bool Pair::forEach(const std::function<bool(Value*, bool)>& function) {
    Value* pair = this;
    bool wantMore = true;
    while(!pair->isNil() && wantMore){
        auto* remaining = pair->getRight();
        wantMore = function(pair->getLeft(), remaining->isNil());
        pair = remaining;
    }
    return wantMore;
}

Value *Symbol::evaluate(Context *context) {
    Value* defined = context->getValue(*this);
    if(defined != nullptr){
        return defined;
    }else{
        return new Nil();
    }
}

void Symbol::evaluate(Context *context, const Continuation &continuation) {
    Value* defined = context->getValue(*this);
    continuation(defined == nullptr ? new Nil() : defined);
}

};
