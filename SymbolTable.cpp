#include "SymbolTable.hpp"
#include "hw3_output.hpp"
using namespace output;

SymbolTable::SymbolTable(int init_offset){
    this->curr_offset = init_offset;
    this->table = vector<Symbol>();
}

void SymbolTable::addSymbol(string name, string type, bool is_func, vector<Symbol> params){
    if (this->symbolExists(name)){
        printf("Symbol %s already exists", name);
    }
    else{
        if (is_func){
            this->table.push_back(Symbol(type, name, -1, true, params));
            for (int i=0; i<params.size();i++){
                this->table.push_back(Symbol(params.at(i).getType(), params.at(i).getName(), -(i + 1), false, vector<Symbol>()));
            }
        }
        else{
            this->table.push_back(Symbol(type, name, curr_offset, false, vector<Symbol>()));
            int size = 0;
            curr_offset += 1;
        }
    }
}

Symbol* SymbolTable::getSymbol(string name){
    for (int i=0; i<this->table.size(); i++){
        if (this->table.at(i).getName() == name){
            return &(this->table.at(i));
        }
    }
    return nullptr;
}

bool SymbolTable::symbolExists(string name){
    for (int i=0; i<this->table.size(); i++){
        if (this->table.at(i).getName() == name){
            return true;
        }
    }
    return false;
}

void SymbolTable::printSymbols(){
    for (int i=0; i<this->table.size(); i++){
        printID(this->table[i].getName(), this->table[i].getOffset(), this->table[i].getType());
    }
}
