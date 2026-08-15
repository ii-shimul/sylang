cmake_minimum_required(VERSION 3.10)
project(sylang CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sylang
    main.cpp
    lexer/Lexer.cpp
    lexer/Lexer.h
    token/Token.cpp
    token/Token.h
    parser/Parser.cpp
    parser/Parser.h
    semantic/SymbolTable.cpp
    semantic/SymbolTable.h
    semantic/SemanticAnalyser.cpp
    semantic/SemanticAnalyser.h
)
