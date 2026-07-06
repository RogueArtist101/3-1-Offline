#ifndef SYMBOLINFO_H
#define SYMBOLINFO_H

#include <string>
#include <iostream>
using namespace std;

struct ParamPair {
    string first;  
    string second; 
};

class SymbolInfo {
    private:
        string name;
        string type;    
        SymbolInfo* next;

        string returnType;
        ParamPair* params;
        int paramCount;
        int paramCapacity;

    public:
        SymbolInfo(string name, string type);
        ~SymbolInfo();

        string getName();
        void setName(string name);
        string getType();
        void setType(string type);
        SymbolInfo* getNext();
        void setNext(SymbolInfo* next);

        void setReturnType(string rt);
        string getReturnType();

        void addParam(string first, string second);
        int getParamCount();
        ParamPair getParam(int index);

        string getFormatted();
};

#endif