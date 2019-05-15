cmake_minimum_required(VERSION 3.14)
project(SchemeCpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(SchemeCpp main.cpp expression.h value.h values.h pch.h interpreter.h context.h continuation.h values.cpp exception.h lexer.h token.h parser.h vector_reader.h)