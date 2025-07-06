cmake_minimum_required(VERSION 3.29)
project(rusty)

set(CMAKE_CXX_STANDARD 20)

add_executable(rusty main.cpp
        src/lexic/Scanner.cpp
        src/lexic/Token.cpp
        src/semantic/CodeGen.cpp
        src/semantic/NameRes.cpp
        src/semantic/Printer.cpp
        src/semantic/SymbolTable.cpp
        src/semantic/TypeCheck.cpp
        src/semantic/Visitor.cpp
        src/syntactic/Exp.cpp
        src/syntactic/Fun.cpp
        src/syntactic/Parser.cpp
        src/syntactic/Stmt.cpp)
