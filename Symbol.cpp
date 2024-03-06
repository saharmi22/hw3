#include "Symbol.hpp"

Symbol::Symbol(string type, string name, int offset, bool is_func, vector<Symbol> params)
{
    this->type = type;
    this->name = name;
    this->offset = offset;
    this->func = is_func;
    this->params = params;
}

string Symbol::getName(){
    return this->name;
}

string Symbol::getType(){
    return this->type;
}

bool Symbol::isFunc(){
    return this->func;
}

vector<Symbol> Symbol::getFuncParams(){
    return this->params;
}