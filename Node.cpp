#include "Node.hpp"
#include "hw3_output.hpp"
#include <typeinfo>
using namespace output;

extern TableStack stack;
extern int yylineno;

#define DEBUG true

// Node Implementation

Node::Node(string n){
    this->name = n;
}

Node::Node(){
    this->name = "";
}

string Node::getName(){
    return this->name;
}

// Statement Implementation

Statement::Statement(Node* s){
    
    if (typeid(*s) == typeid(Statements)){
        string rule = "LBRACE Statments RBRACE";
        this->statements = (Statements*)(s);
        endScope();
        stack.printScope();
        stack.popTableFromStack();
    }

    else{
        string rule = "Call SC";
        this->call = (Call*)(s);
    }
    
}

Statement::Statement(Node* t_e, Node* id){
    if (typeid(*t_e) == typeid(Type)){
        string rule = "Type ID SC";
        this->type = (Type*)(t_e);
        this->node = id;
        if (stack.symbolExists(id->getName())){
            errorDef(yylineno, id->getName());
            exit(0);
        }
        stack.addSymbol(this->type->getType(), id->getName(), false, vector<Symbol>());
    }
    else{
        string rule = "ID ASSIGN Exp SC";
        this->node = id;
        this->exp = (Expression*)(t_e);
        if (!stack.symbolExists(id->getName())){
            errorUndef(yylineno, id->getName());
            exit(0);
        }
    }
}

Statement::Statement(Node* t, Node* id, Node* e){
    if (typeid(*t) == typeid(Type)){
        string rule = "Type ID ASSIGN Exp SC";
        this->type = (Type*)(t);
        this->node = id;
        this->exp = (Expression*)(e);
        if (stack.symbolExists(id->getName())){
            errorDef(yylineno, id->getName());
            exit(0);
        }
        if (DEBUG){
            printf("identified defenition + assignment");
        }
        if (this->exp->getExpType() != this->type->getType()){
            if (!(this->type->getType() == "INT" && this->exp->getExpType() == "BYTE")){
                errorMismatch(yylineno);
                exit(0);
            }
        }
        stack.addSymbol(this->type->getType(), id->getName(), false, vector<Symbol>());
    }
    else{
        if (DEBUG){
            printf("hi");
        }
        string rule = "IF LPAREN Exp RPAREN Statment ELSE Statment";
        this->exp = (Expression*)(e);
        this->statement1 = (Statement*)(t);
        this->statement2 = (Statement*)(id);
        if (this->exp->getExpType() == "BOOL"){
            endScope();
            stack.printScope();
           stack.popTableFromStack();
            endScope();
            stack.printScope();
            stack.popTableFromStack();
        }
        else{
            errorMismatch(yylineno);
            exit(0);
        }
    }
}

Statement::Statement(string type){
    if (type == "return"){
        string rule = "RETURN SC";
    }
    if (type == "continue"){
        string rule = "CONTINUE SC";
        this->is_loop_statment = true;
        this->loop_statement = "CONTINUE";
    }
    if (type == "break"){
        string rule = "BREAK SC";
        this->is_loop_statment = true;
        this->loop_statement = "BREAK";
    }
}

Statement::Statement(Node* e, Node* s, bool is_if){
    string rule = "IF LPAREN Exp RPAREN Statment";
    if (!is_if){
        rule = "WHILE LPAREN Exp RPAREN Statment";
        ((Statement*)(s))->is_in_loop = true;
    }
    else{
        ((Statement*)(s))->is_in_loop = false;
    }
    ((Statement*)(s))->checkLoopStatement();
    this->exp = (Expression*)e;
    this->statement1 = ((Statement*)(s));
    if (((Expression*)e)->getExpType() == "BOOL"){
        endScope();
        stack.printScope();
        stack.popTableFromStack();
    }
    else{
        errorMismatch(yylineno);
        exit(0);
    }
}

bool Statement::checkLoopStatement(){
    if (this->is_loop_statment){
        if (this->is_in_loop){
            return true;
        }
        if(loop_statement == "CONTINUE")
            errorUnexpectedContinue(yylineno);
        else
            errorUnexpectedBreak(yylineno);
        exit(0);
    }
    return true;
}

// Statements Implementation

Statements::Statements(Node* s2, Node* s1){
    this->statements = (Statements*)s1;
    this->statement = (Statement*)s2;
    statement->is_in_loop = false;
    statement->checkLoopStatement();
}

Statements::Statements(Node* s1){
    this->statement = (Statement*)s1;
    this->statements = nullptr;
    statement->is_in_loop = false;
    statement->checkLoopStatement();
}

// Program Implementation

Program::Program(Node* s){
    this->s = (Statements*)(s);
    endScope();
    stack.printScope();
    stack.popTableFromStack();
}


// Call Implementation

Call::Call(Node* id, Node* exp){
    string rule = "ID LPAREN Exp RPAREN";
    this->id = id;
    this->exp = (Expression*)exp;
    Symbol* s = stack.getSymbol(id->getName());
    if (s){
        if(s->isFunc()){
            if (s->getName() == "print"){
                this->ret_type = "VOID";
                if (this->exp->getExpType() != "STRING"){
                    errorPrototypeMismatch(yylineno, "print", "STRING");
                    exit(0);
                }
            }
            else if (s->getName() == "printi"){
                this->ret_type = "VOID";
                if (this->exp->getExpType() != "INT"){
                    errorPrototypeMismatch(yylineno, "printi", "INT");
                    exit(0);
                }
            }
            else{
                this->ret_type = "INT";
                if (this->exp->getExpType() != "INT"){
                    errorPrototypeMismatch(yylineno, "readi", "INT");
                    exit(0);
                }
            }
            
        }
        else{
            errorUndefFunc(yylineno, id->getName());
            exit(0);
        }
    }
    else{
        errorUndefFunc(yylineno, id->getName());
        exit(0);
    }
}

// Type Implementation

Type::Type(string type){
    string rule = "INT";
    if (type == "BOOL") {
        rule = "BOOL";
    }
    if (type == "BYTE") {
        rule = "BYTE";
    }
    this->type = type;
}

string Type::getType()
{
    return this->type;
}

// Expression Implementation

Expression::Expression(Node* n_exp, bool is_parens){
    Expression* exp = (Expression*)(n_exp);
    string rule = "LPAREN Exp RPAREN";
    if (!is_parens){
        rule = "NOT Exp";
        if (exp->getExpType() != "BOOL"){
            errorMismatch(yylineno);
            exit(0);
        }
    }
    this->exp1 = exp;
    this->exp_type = exp->getExpType();
}

Expression::Expression(Node* n_exp1, Node* n_exp2, string middle_word){
    Expression* exp1 = (Expression*)(n_exp1);
    Expression* exp2 =(Expression*)(n_exp2);
    this->exp1 = exp1;
    this->exp2 = exp2;
    if (middle_word == "MINUS" || middle_word == "PLUS" || middle_word == "MULT" || middle_word == "DIV"){
        if (exp1->getExpType() == "INT" && exp1->getExpType() == "INT"){
            this->exp_type = "INT";
        }
        if (exp1->getExpType() == "INT" && exp1->getExpType() == "BYTE"){
            this->exp_type = "INT";
        }
        if (exp1->getExpType() == "BYTE" && exp1->getExpType() == "INT"){
            this->exp_type = "INT";
        }
        if (exp1->getExpType() == "BYTE" && exp1->getExpType() == "BYTE"){
            this->exp_type = "BYTE";
        }
        else{
            errorMismatch(yylineno);
            exit(0);
        }
    }
    else if (middle_word == "AND" || middle_word == "OR"){
        if (exp1->getExpType() == "BOOL" && exp1->getExpType() == "BOOL"){
            this->exp_type = "BOOL";
        }
        else{
            errorMismatch(yylineno);
            exit(0);
        }
    }
    else if (middle_word == "REL" || middle_word == "EQ"){
        if ((exp1->getExpType() == "INT" || exp1->getExpType() == "BYTE") && (exp2->getExpType() == "INT" || exp2->getExpType() == "BYTE")){
            this->exp_type = "BOOL";
        }
        else{
            errorMismatch(yylineno);
            exit(0);
        }
    }
}

Expression::Expression(Node* n_c){
    if (typeid(*n_c) == typeid(Call)){
        Call* c = (Call*)(n_c);
        string rule = "Call";
        this->call = c;
    }
    else{
        string rule = "ID";
        this->id = n_c;
        if (stack.symbolExists(n_c->getName())){
            errorUndef(yylineno, n_c->getName());
            exit(0);
        }
    }
}

Expression::Expression(int num, bool is_byte){
    string rule = "NUM";
    this->exp_type = "INT";
    if (is_byte){
        rule = "NUM B";
        if (0 <= num && num <= 255){
            this->exp_type = "BYTE";
        }
        else{
            errorByteTooLarge(yylineno, to_string(num));
        }
    }
    if (DEBUG){
        printf("identified number");
    }
    this->num = num;
}

Expression::Expression(Node* n_type, Node* n_exp){
    Type* type = (Type*)(n_type);
    Expression* exp = (Expression*)(n_exp);
    string rule = "LPAREN Type RPAREN Exp";
    this->type = type;
    this->exp1 = exp;
    if ((exp->getExpType() == "INT" || exp->getExpType() == "BYTE") && (type->getType() == "INT" || type->getType() == "BOOL")){
        this->exp_type = type->getType();
    }
    else{
        errorMismatch(yylineno);
        exit(0);
    }    
}

Expression::Expression(string string_true_false){
    string rule = "";
    
    if (string_true_false == "TRUE"){
        rule = "TRUE";
        this->exp_type = "BOOL";
    }
    else if (string_true_false == "FALSE"){
        rule = "FALSE";
        this->exp_type = "BOOL";
    }
    else if (string_true_false == "STRING"){
        rule = "STRING";
        this->exp_type = "STRING";
    }
    else{
        errorMismatch(yylineno);
        exit(0);
    }
    this->string_true_false = string_true_false;
}

string Expression::getExpType(){
    return this->exp_type;
}
