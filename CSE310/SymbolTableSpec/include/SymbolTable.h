#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "ScopeTable.h"
#include <bits/stdc++.h>
using namespace std;

class SymbolTable {
    private:
        ScopeTable* currentScope;
        int numBuckets;
        int nextId;

    public:
        SymbolTable(int numBuckets);
        void EnterScope();
        void ExitScope();
        bool Insert(string name, string type);
        bool Remove(string name);
        SymbolInfo* LookUp(string name);
        void PrintCurrentScope();
        void PrintAllScopes();
        ~SymbolTable();
};


#endif
