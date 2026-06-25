#include "../include/SymbolInfo.h"

SymbolInfo::SymbolInfo(string name, string type) {
    this->name = name;
    this->type = type;
    this->next = nullptr;
}

SymbolInfo::~SymbolInfo() {
    //see you later
}

string SymbolInfo::getName() {
    return this->name;
}
void SymbolInfo::setName(std::string name) { 
    this->name = name; 
}
string SymbolInfo::getType() { 
    return type; 
}

void SymbolInfo::setType(std::string type) { 
    this->type = type; 
}

SymbolInfo* SymbolInfo::getNext() { 
    return this->next; 
}

void SymbolInfo::setNext(SymbolInfo* next) { 
    this->next = next; 
}