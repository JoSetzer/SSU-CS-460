cmake_minimum_required(VERSION 4.0)
project(SSU_CS_460)

set(CMAKE_CXX_STANDARD 26)

include_directories(interpreter_phase1)

add_executable(SSU_CS_460
        interpreter_phase1/Expr.cpp
        interpreter_phase1/Expr.hpp
        interpreter_phase1/main.cpp
        interpreter_phase1/Parser.cpp
        interpreter_phase1/Parser.hpp
        interpreter_phase1/Statements.cpp
        interpreter_phase1/Statements.hpp
        interpreter_phase1/SymbolTable.cpp
        interpreter_phase1/SymbolTable.hpp
        interpreter_phase1/Token.cpp
        interpreter_phase1/Token.hpp
        interpreter_phase1/Tokenizer.cpp
        interpreter_phase1/Tokenizer.hpp)
