#include "../include/2205078_SymbolTable.h"

SymbolTable::SymbolTable(int numBuckets, ostream& out) {
    this->numBuckets = numBuckets;
    this->currentScope = nullptr;
    this->nextId = 1;

    EnterScope(out);
}

SymbolTable::~SymbolTable() {
    while (currentScope != nullptr) {
        ScopeTable* temp = currentScope;
        currentScope = currentScope->getParentScope();
        delete temp;
    }
}

void SymbolTable::EnterScope(ostream& out) {
    ScopeTable* newScope = new ScopeTable(numBuckets, nextId, currentScope);
    out << "\tScopeTable# " << nextId << " created" << endl;
    nextId++;
    currentScope = newScope;
}

void SymbolTable::ExitScope(ostream& out) {
    if (currentScope != nullptr) {
        out << "\tScopeTable# " << currentScope->getId() << " removed" << endl;
        ScopeTable* temp = currentScope;
        currentScope = currentScope->getParentScope();
        delete temp;
    }
}

bool SymbolTable::Insert(string name, string type, ostream& out) {
    if (currentScope != nullptr) {
        return currentScope->Insert(name, type, out);
    }
    return false;
}

bool SymbolTable::Remove(string name, ostream& out) {
    if (currentScope != nullptr) {
        return currentScope->Delete(name, out);
    }
    return false;
}

SymbolInfo* SymbolTable::LookUp(string name, ostream& out) {
    ScopeTable* scope = currentScope;
    while (scope != nullptr) {
        int bucketPos, chainPos;
        SymbolInfo* symbol = scope->LookUp(name, bucketPos, chainPos);
        if (symbol != nullptr) {
            out << "\t'" << name << "' found in ScopeTable# " << scope->getId()
                << " at position " << bucketPos << ", " << chainPos << endl;
            return symbol;
        }
        scope = scope->getParentScope();
    }
    out << "\t'" << name << "' not found in any of the ScopeTables" << endl;
    return nullptr;
}

void SymbolTable::PrintCurrentScope(ostream& out) {
    if (currentScope != nullptr) {
        currentScope->Print(out, 1);
    }
}

void SymbolTable::PrintAllScopes(ostream& out) {
    ScopeTable* scope = currentScope;
    int indent = 1;
    while (scope != nullptr) {
        scope->Print(out, indent);
        scope = scope->getParentScope();
        indent++;
    }
}

ScopeTable* SymbolTable::getCurrentScope() {
    return currentScope;
}

bool SymbolTable::isAtRootScope() {
    if (currentScope == nullptr) return true;
    return (currentScope->getParentScope() == nullptr);
}
