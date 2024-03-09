#include <vector>
#include "TableStack.hpp"

TableStack::TableStack(){
    this->table_stack = vector<SymbolTable>();
    this->offset_stack = vector<int>();
    this->offset_stack.push_back(0);
}

void TableStack::addTableToStack(){
    this->table_stack.push_back(SymbolTable(this->offset_stack.at(0)));
}

SymbolTable TableStack::getLastTable(){
    return this->table_stack.back();
}

void TableStack::popTableFromStack()
{
    this->table_stack.pop_back();
}

void TableStack::addSymbol(string name, string type, bool is_func, vector<Symbol> params){
    SymbolTable cur_table = this->table_stack.back();
    cur_table.addSymbol(name, type, is_func, params);
    this->popTableFromStack();
    this->table_stack.push_back(cur_table);
    if (!is_func){
        this->offset_stack.back()++;
    }
}

bool TableStack::symbolExists(string name){
    for (int i=this->table_stack.size()-1; i >=0; i--){
        if (this->table_stack.at(i).symbolExists(name)){
            return true;
        }
    }
    return false;
}

Symbol* TableStack::getSymbol(string name){
    for (int i = this->table_stack.size()-1; i >= 0; i--){
        if (this->table_stack.at(i).symbolExists(name)){
            return (this->table_stack.at(i).getSymbol(name));
        }
    }
    return nullptr;
}

void TableStack::printScope(){
    SymbolTable t = this->getLastTable();
    t.printSymbols();
}