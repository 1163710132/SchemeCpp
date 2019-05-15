cmake_minimum_required(VERSION 3.14)
project(SchemeCpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(SchemeCpp main.cpp expression.h value.h values.h pch.h interpreter.h context.h continuation.h values.cpp exception.h)