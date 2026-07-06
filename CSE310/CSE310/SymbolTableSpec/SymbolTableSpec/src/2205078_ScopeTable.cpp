#include "../include/2205078_ScopeTable.h"

ScopeTable::ScopeTable(int numBuckets, int id, ScopeTable* parentScope) {
    this->numBuckets = numBuckets;
    this->id = id;
    this->parentScope = parentScope;
    this->table = new SymbolInfo*[numBuckets];
    for (int i = 0; i < numBuckets; i++) {
        table[i] = nullptr;
    }
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

SymbolInfo* ScopeTable::LookUp(string name, int& bucketPos, int& chainPos) {
    unsigned int index = SDBMHash(name);
    bucketPos = (int)index + 1;
    chainPos = 1;
    SymbolInfo* current = table[index];
    while (current != nullptr) {
        if (current->getName() == name) {
            return current;
        }
        current = current->getNext();
        chainPos++;
    }
    chainPos = 0;
    return nullptr;
}

SymbolInfo* ScopeTable::LookUp(string name) {
    int bp, cp;
    return LookUp(name, bp, cp);
}

bool ScopeTable::Insert(string name, string type, ostream& out) {
    int bp, cp;
    if (LookUp(name) != nullptr) {
        return false;  // already exists
    }

    unsigned int idx = SDBMHash(name);
    SymbolInfo* newSymbol = new SymbolInfo(name, type);

    // Append at end of chain and count position
    int chainPos = 1;
    if (table[idx] == nullptr) {
        table[idx] = newSymbol;
    } else {
        SymbolInfo* current = table[idx];
        chainPos = 2;
        while (current->getNext() != nullptr) {
            current = current->getNext();
            chainPos++;
        }
        current->setNext(newSymbol);
    }

    out << "\tInserted in ScopeTable# " << this->id
        << " at position " << (idx + 1) << ", " << chainPos << endl;

    return true;
}

// Delete from this scope table
bool ScopeTable::Delete(string name, ostream& out) {
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

            out << "\tDeleted '" << name << "' from ScopeTable# " << this->id
                << " at position " << (index + 1) << ", " << pos << endl;
            return true;
        }
        previous = current;
        current = current->getNext();
        pos++;
    }

    out << "\tNot found in the current ScopeTable" << endl;
    return false;
}

// Print with indentation
void ScopeTable::Print(ostream& out, int indentLevel) {
    // Build indent string
    string indent = "";
    for (int i = 0; i < indentLevel; i++) {
        indent += "\t";
    }

    out << indent << "ScopeTable# " << this->id << endl;

    for (int i = 0; i < numBuckets; i++) {
        out << indent << (i + 1) << "--> ";

        SymbolInfo* current = table[i];
        while (current != nullptr) {
            out << current->getFormatted() << " ";
            current = current->getNext();
        }
        out << endl;
    }
}