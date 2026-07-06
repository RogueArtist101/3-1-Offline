#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "2205078_ScopeTable.h"
#include <string>
#include <iostream>
using namespace std;

class SymbolTable {
    private:
        ScopeTable* currentScope;
        int numBuckets;
        int nextId;

    public:
        SymbolTable(int numBuckets, ostream& out);
        ~SymbolTable();

        void EnterScope(ostream& out);
        void ExitScope(ostream& out);
        bool Insert(string name, string type, ostream& out);
        bool Remove(string name, ostream& out);
        SymbolInfo* LookUp(string name, ostream& out);
        void PrintCurrentScope(ostream& out);
        void PrintAllScopes(ostream& out);

        ScopeTable* getCurrentScope();

        bool isAtRootScope();
};

#endif
