#include "../include/2205078_SymbolTable.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

static string rtrim(const string& s) {
    int end = (int)s.size() - 1;
    while (end >= 0 && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r' || s[end] == '\n')) {
        end--;
    }
    return s.substr(0, end + 1);
}

int main(int argc, char* argv[]) {
    ifstream fin;
    ofstream fout;

    if (argc >= 3) {
        fin.open(argv[1]);
        fout.open(argv[2]);
    } else if (argc == 2) {
        fin.open(argv[1]);
    }

    istream& in = (argc >= 2 && fin.is_open()) ? (istream&)fin : cin;
    ostream& out = (argc >= 3 && fout.is_open()) ? (ostream&)fout : cout;

    int numBuckets;
    in >> numBuckets;
    in.ignore(); 

    SymbolTable st(numBuckets, out);

    string line;
    int cmdNum = 0;

    while (getline(in, line)) {
        line = rtrim(line);

        if (line.empty()) continue;

        istringstream iss(line);
        string cmd;
        iss >> cmd;

        if (cmd.empty()) continue;

        char command = cmd[0];

        if (command == 'I') {
            string name, type;
            iss >> name >> type;

            if (name.empty() || type.empty()) {
                cmdNum++;
                out << "Cmd " << cmdNum << ": " << line << endl;
                out << "\tNumber of parameters mismatch for the command I" << endl;
                continue;
            }

            cmdNum++;
            out << "Cmd " << cmdNum << ": " << line << endl;

            if (type == "FUNCTION") {
                string returnType;
                iss >> returnType;

                bool inserted = st.Insert(name, type, out);
                if (!inserted) {
                    out << "\t'" << name << "' already exists in the current ScopeTable" << endl;
                } else {
                    int bp, cp;
                    SymbolInfo* sym = st.getCurrentScope()->LookUp(name, bp, cp);
                    if (sym != nullptr) {
                        sym->setReturnType(returnType);
                        string paramType;
                        while (iss >> paramType) {
                            sym->addParam(paramType, "");
                        }
                    }
                }
            } else if (type == "STRUCT" || type == "UNION") {
                bool inserted = st.Insert(name, type, out);
                if (!inserted) {
                    out << "\t'" << name << "' already exists in the current ScopeTable" << endl;
                } else {
                    int bp, cp;
                    SymbolInfo* sym = st.getCurrentScope()->LookUp(name, bp, cp);
                    if (sym != nullptr) {
                        string memberType, memberName;
                        while (iss >> memberType >> memberName) {
                            sym->addParam(memberType, memberName);
                        }
                    }
                }
            } else {
                bool inserted = st.Insert(name, type, out);
                if (!inserted) {
                    out << "\t'" << name << "' already exists in the current ScopeTable" << endl;
                }
            }
        } else if (command == 'L') {
            string name;
            iss >> name;

            string extra;
            if (name.empty()) {
                cmdNum++;
                out << "Cmd " << cmdNum << ": " << line << endl;
                out << "\tNumber of parameters mismatch for the command L" << endl;
                continue;
            }

            if (iss >> extra) {
                cmdNum++;
                out << "Cmd " << cmdNum << ": " << line << endl;
                out << "\tNumber of parameters mismatch for the command L" << endl;
                continue;
            }

            cmdNum++;
            out << "Cmd " << cmdNum << ": " << line << endl;
            st.LookUp(name, out);

        } else if (command == 'D') {
            string name;
            iss >> name;

            if (name.empty()) {
                cmdNum++;
                out << "Cmd " << cmdNum << ": " << line << endl;
                out << "\tNumber of parameters mismatch for the command D" << endl;
                continue;
            }

            string extra;
            if (iss >> extra) {
                cmdNum++;
                out << "Cmd " << cmdNum << ": " << line << endl;
                out << "\tNumber of parameters mismatch for the command D" << endl;
                continue;
            }

            cmdNum++;
            out << "Cmd " << cmdNum << ": " << line << endl;
            st.Remove(name, out);

        } else if (command == 'P') {
            string option;
            iss >> option;

            if (option == "A") {
                cmdNum++;
                out << "Cmd " << cmdNum << ": " << line << endl;
                st.PrintAllScopes(out);
            } else if (option == "C") {
                cmdNum++;
                out << "Cmd " << cmdNum << ": " << line << endl;
                st.PrintCurrentScope(out);
            }

        } else if (command == 'S') {
            cmdNum++;
            out << "Cmd " << cmdNum << ": " << line << endl;
            st.EnterScope(out);

        } else if (command == 'E') {
            if (st.isAtRootScope()) {
                continue;
            }
            cmdNum++;
            out << "Cmd " << cmdNum << ": " << line << endl;
            st.ExitScope(out);

        } else if (command == 'Q') {
            cmdNum++;
            out << "Cmd " << cmdNum << ": " << line << endl;
            while (!st.isAtRootScope()) {
                st.ExitScope(out);
            }
            st.ExitScope(out);
            break;

        }
    }

    if (fin.is_open()) fin.close();
    if (fout.is_open()) fout.close();

    return 0;
}
