#ifndef SYMBOLINFO_H
#define SYMBOLINFO_H

#include <bits/stdc++.h>
using namespace std;

class SymbolInfo {
    private:
        string name;
        string type;
        SymbolInfo* next;
    public: 
        SymbolInfo(string name, string type);
        string getName();
        void setName(string name);
        string getType();
        void setType(string type);
        void setNext(SymbolInfo* next);
        SymbolInfo* getNext();
        ~SymbolInfo();
};

#endif