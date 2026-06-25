#include "../include/SymbolTable.h"

SymbolTable::SymbolTable(int numBuckets) {
    this->numBuckets = numBuckets;
    this->currentScope = nullptr;
    this->nextId = 1;   

    EnterScope();
}   

SymbolTable::~SymbolTable() {
    while (currentScope != nullptr) {
        ExitScope();
    }
}

void SymbolTable::EnterScope() {
    ScopeTable* newScope = new ScopeTable(numBuckets, nextId++, currentScope);
    this->nextId++;
    currentScope = newScope;
}

void SymbolTable::ExitScope() {
    if (currentScope != nullptr) {
        ScopeTable* temp = currentScope;
        currentScope = currentScope->getParentScope();
        delete temp;
    }
}

bool SymbolTable::Insert(string name, string type) {
    if (currentScope != nullptr) {
        return currentScope->Insert(name, type);
    }
    return false;
}

bool SymbolTable::Remove(string name) {
    if (currentScope != nullptr) {
        return currentScope->Delete(name);
    }
    return false;
}

SymbolInfo* SymbolTable::LookUp(string name) {
    ScopeTable* scope = currentScope;
    while (scope != nullptr) {
        SymbolInfo* symbol = scope->LookUp(name);
        if (symbol != nullptr) {
            return symbol;
        }
        scope = scope->getParentScope();
    }
    cout << "\t'" << name << "' not found in any of the ScopeTables" << endl;
    return nullptr;
}

void SymbolTable::PrintCurrentScope() {
    if (currentScope != nullptr) {
        currentScope->Print();
    }
}

void SymbolTable::PrintAllScopes() {
    ScopeTable* scope = currentScope;
    while (scope != nullptr) {
        scope->Print();
        scope = scope->getParentScope();
    }
}
