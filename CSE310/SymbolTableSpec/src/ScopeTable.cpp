#include "../include/ScopeTable.h"

ScopeTable::ScopeTable(int numBuckets, int id, ScopeTable* parentScope) {
    this->numBuckets = numBuckets;
    this->id = id;
    this->parentScope = parentScope;
    this->table = new SymbolInfo*[numBuckets];
    for (int i = 0; i < numBuckets; i++) {
        table[i] = nullptr;
    }

    cout << "ScopeTable with id " << this->id << " created" << endl;
}

ScopeTable::~ScopeTable() {
    for (int i = 0; i < numBuckets; i++) {
        SymbolInfo* current = table[i];
        while (current != nullptr) {
            SymbolInfo* temp = current;
            current = current->getNext();
            delete temp;
        }
    }
    delete[] table;
    cout << "ScopeTable with id " << this->id << " removed" << endl;
}

unsigned int ScopeTable::SDBMHash(string str) {
    unsigned int hash = 0;
    unsigned int len = str.length();
    for (unsigned int i = 0; i < len; i++) {
        hash = ((str[i]) + (hash << 6) + (hash << 16) - hash) % this->numBuckets;
    }
    
    return hash;
}

ScopeTable* ScopeTable::getParentScope() {
    return this->parentScope;
}

int ScopeTable::getId() {
    return this->id;
}

SymbolInfo* ScopeTable::LookUp(string name) {
    unsigned int index = SDBMHash(name);
    SymbolInfo* current = table[index];
    while (current != nullptr) {
        if (current->getName() == name) {
            return current;
        }
        current = current->getNext();
    }
    return nullptr;
}

bool ScopeTable::Insert(string name, string type) {
    if(LookUp(name) != nullptr) {
        return false;
    }

    unsigned int idx = SDBMHash(name);

    SymbolInfo* newSymbol = new SymbolInfo(name, type);
    newSymbol->setNext(table[idx]);

    int pos = 1;

    if(table[idx] == nullptr) {
        table[idx] = newSymbol;
    } else {
        SymbolInfo* current = table[idx];
        while (current->getNext() != nullptr) {
            current = current->getNext();
            pos++;
        }
        current->setNext(newSymbol);
    }

    cout << "Inserted in ScopeTable with id " << this->id << " at position " << idx << ", " << pos << endl;
    
    return true;
}

bool ScopeTable::Delete(string name) {
    unsigned int index = SDBMHash(name);
    
    SymbolInfo* current = table[index];
    SymbolInfo* previous = nullptr;
    int pos = 1;

    while (current != nullptr) {
        if (current->getName() == name) {
            
            if (previous == nullptr) {
                table[index] = current->getNext();
            } else {
                previous->setNext(current->getNext());
            }
            
            delete current;
            
            cout << "\tDeleted '" << name << "' from ScopeTable with id " << this->id << " at position " << (index + 1) << ", " << pos << endl;
            return true;
        }
        
        previous = current;
        current = current->getNext();
        pos++;
    }

    cout << "\tNot found in the current ScopeTable" << endl;
    return false;
}


void ScopeTable::Print() {
    cout << "\tScopeTable with id " << this->id << endl;
    
    for (int i = 0; i < numBuckets; i++) {
        cout << "\t" << (i + 1) << "--> ";
        
        SymbolInfo* current = table[i];
        while (current != nullptr) {
            cout << "<" << current->getName() << "," << current->getType() << "> ";
            current = current->getNext();
        }
        cout << endl;
    }
}