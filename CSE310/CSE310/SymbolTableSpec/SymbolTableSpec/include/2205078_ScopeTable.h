#ifndef SCOPETABLE_H
#define SCOPETABLE_H

#include "2205078_SymbolInfo.h"
#include <string>
#include <iostream>
using namespace std;

class ScopeTable {
    private:
        SymbolInfo** table;
        int numBuckets;
        ScopeTable* parentScope;
        int id;

        unsigned int SDBMHash(string str);

    public:
        ScopeTable(int numBuckets, int id, ScopeTable* parentScope);
        ~ScopeTable();

        bool Insert(string name, string type, ostream& out);
        SymbolInfo* LookUp(string name, int& bucketPos, int& chainPos);
        SymbolInfo* LookUp(string name);
        bool Delete(string name, ostream& out);
        void Print(ostream& out, int indentLevel);

        ScopeTable* getParentScope();
        int getId();
};

#endif