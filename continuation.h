//
// Created by chenjs on 19-5-15.
//

#ifndef SCHEMECPP_CONTINUATION_H
#define SCHEMECPP_CONTINUATION_H

#include <functional>

namespace scheme{

class Value;

typedef std::function<void(Value*)> Continuation;

}

#endif //SCHEMECPP_CONTINUATION_H
