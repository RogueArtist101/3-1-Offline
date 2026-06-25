#ifndef SCOPETABLE_H
#define SCOPETABLE_H

#include <SymbolInfo.h>

#include<bits/stdc++.h>
using namespace std;

class ScopeTable {
    private :
        SymbolInfo** table;
        int numBuckets;
        ScopeTable* parentScope;
        int id;

        unsigned int SDBMHash(string str);

    public :
        ScopeTable(int numBuckets, int id, ScopeTable* parentScope);
        bool Insert(string name, string type);
        bool Delete(string name);
        SymbolInfo* LookUp(string name);
        void Print();

        ScopeTable* getParentScope();
        int getId();
        ~ScopeTable();

};


#endif