//
// Created by chenjs on 19-5-16.
//

#ifndef SCHEMECPP_VECTOR_READER_H
#define SCHEMECPP_VECTOR_READER_H

#include <vector>

template <typename T>
class VectorReader{
public:
    size_t pos;
    std::vector<T> vector;

public:
    T& peek(){
        return vector[pos];
    }
};

#endif //SCHEMECPP_VECTOR_READER_H
