#include "../include/2205078_SymbolInfo.h"

SymbolInfo::SymbolInfo(string name, string type) {
    this->name = name;
    this->type = type;
    this->next = nullptr;
    this->returnType = "";
    this->params = nullptr;
    this->paramCount = 0;
    this->paramCapacity = 0;
}

SymbolInfo::~SymbolInfo() {
    if (params != nullptr) {
        delete[] params;
    }
}

string SymbolInfo::getName() {
    return this->name;
}

void SymbolInfo::setName(string name) {
    this->name = name;
}

string SymbolInfo::getType() {
    return this->type;
}

void SymbolInfo::setType(string type) {
    this->type = type;
}

SymbolInfo* SymbolInfo::getNext() {
    return this->next;
}

void SymbolInfo::setNext(SymbolInfo* next) {
    this->next = next;
}

void SymbolInfo::setReturnType(string rt) {
    this->returnType = rt;
}

string SymbolInfo::getReturnType() {
    return this->returnType;
}

void SymbolInfo::addParam(string first, string second) {
    if (paramCount == paramCapacity) {
        int newCap = (paramCapacity == 0) ? 4 : paramCapacity * 2;
        ParamPair* newArr = new ParamPair[newCap];
        for (int i = 0; i<paramCount; i++) {
            newArr[i] = params[i];
        }
        if (params != nullptr) {
            delete[] params;
        }
        params = newArr;
        paramCapacity = newCap;
    }
    params[paramCount].first = first;
    params[paramCount].second = second;
    paramCount++;
}

int SymbolInfo::getParamCount() {
    return this->paramCount;
}

ParamPair SymbolInfo::getParam(int index) {
    return this->params[index];
}

string SymbolInfo::getFormatted() {
    string result = "<" + name + "," + type;

    if (type == "FUNCTION") {
        result += "," + returnType + "<==(";
        for (int i = 0; i < paramCount; i++) {
            if (i > 0) result += ",";
            result += params[i].first;
        }
        result += ")";
    } else if (type == "STRUCT" || type == "UNION") {
        result += ",{";
        for (int i = 0; i < paramCount; i++) {
            if (i > 0) result += ",";
            result += "(" + params[i].first + "," + params[i].second + ")";
        }
        result += "}";
    }

    result += ">";
    return result;
}
