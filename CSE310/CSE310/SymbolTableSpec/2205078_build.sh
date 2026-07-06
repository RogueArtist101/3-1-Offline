#!/bin/bash

g++ -std=c++17 -I SymbolTableSpec/include \
    SymbolTableSpec/src/2205078_SymbolInfo.cpp \
    SymbolTableSpec/src/2205078_ScopeTable.cpp \
    SymbolTableSpec/src/2205078_SymbolTable.cpp \
    SymbolTableSpec/src/2205078_main.cpp \
    -o 2205078_symboltable

./2205078_symboltable sample_input.txt my_output.txt
