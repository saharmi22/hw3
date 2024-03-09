#include "Node.hpp"
#include "hw3_output.hpp"
using namespace output;

extern TableStack stack;
extern int yylineno;

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

Statement::Statement(Statements* s){
    string rule = "LBRACE Statments RBRACE";
    this->statements = s;
    stack.popTableFromStack();
}

Statement::Statement(Type* t, Node* id){
    string rule = "Type ID SC";
    this->type = t;
    this->node = id;
    if (stack.symbolExists(id->getName())){
        errorDef(yylineno, id->getName());
        exit(0);
    }
    stack.addSymbol(this->type->getType(), id->getName(), false, vector<Symbol>());
}

Statement::Statement(Type* t, Node* id, Expression* e){
    string rule = "Type ID ASSIGN Exp SC";
    this->type = t;
    this->node = id;
    this->exp = e;
    if (stack.symbolExists(id->getName())){
        errorDef(yylineno, id->getName());
        exit(0);
    }
    if (this->exp->getExpType() != this->type->getType()){
        if (!(this->type->getType() == "INT" && this->exp->getExpType() == "BYTE")){
            errorMismatch(yylineno);
            exit(0);
        }
    }
    stack.addSymbol(this->type->getType(), id->getName(), false, vector<Symbol>());
}

Statement::Statement(Node* id, Expression* e){
    string rule = "ID ASSIGN Exp SC";
    this->node = id;
    this->exp = e;
    if (!stack.symbolExists(id->getName())){
        errorUndef(yylineno, id->getName());
        exit(0);
    }
}

Statement::Statement(Call* c){
    string rule = "Call SC";
    this->call = c;
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

Statement::Statement(Expression* e, Statement* s, bool is_if){
    string rule = "IF LPAREN Exp RPAREN Statment";
    if (!is_if){
        rule = "WHILE LPAREN Exp RPAREN Statment";
        s->is_in_loop = true;
    }
    else{
        s->is_in_loop = false;
    }
    s->checkLoopStatement();
    this->exp = e;
    this->statement1 = s;
    if (e->getExpType() == "BOOL"){
        stack.popTableFromStack();
    }
    else{
        errorMismatch(yylineno);
        exit(0);
    }
}

Statement::Statement(Expression* e, Statement* s1, Statement* s2){
    string rule = "IF LPAREN Exp RPAREN Statment ELSE Statment";
    this->exp = e;
    this->statement1 = s1;
    this->statement2 = s2;
    if (e->getExpType() == "BOOL"){
        stack.popTableFromStack();
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

Statements::Statements(Statement* s2, Statements* s1){
    this->statements = s1;
    this->statement = s2;
    statement->is_in_loop = false;
    statement->checkLoopStatement();
}

Statements::Statements(Statement* s1){
    this->statement = s1;
    this->statements = nullptr;
    statement->is_in_loop = false;
    statement->checkLoopStatement();
}

// Program Implementation

Program::Program(Statements* s){
    this->s = s;
    stack.popTableFromStack();
}


// Call Implementation

Call::Call(Node* id, Expression* exp){
    string rule = "ID LPAREN Exp RPAREN";
    this->id = id;
    this->exp = exp;
    Symbol* s = stack.getSymbol(id->getName());
    if (s){
        if(s->isFunc()){
            if (s->getName() == "print"){
                this->ret_type = "VOID";
                if (exp->getExpType() != "STRING"){
                    errorPrototypeMismatch(yylineno, "print", "STRING");
                    exit(0);
                }
            }
            else if (s->getName() == "printi"){
                this->ret_type = "VOID";
                if (exp->getExpType() != "INT"){
                    errorPrototypeMismatch(yylineno, "printi", "INT");
                    exit(0);
                }
            }
            else{
                this->ret_type = "INT";
                if (exp->getExpType() != "INT"){
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
    Expression* exp = dynamic_cast<Expression*>(n_exp);
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
    Expression* exp1 = dynamic_cast<Expression*>(n_exp1);
    Expression* exp2 = dynamic_cast<Expression*>(n_exp2);
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

Expression::Expression(Node* id){
    string rule = "ID";
    this->id = id;
    if (stack.symbolExists(id->getName())){
        errorUndef(yylineno, id->getName());
        exit(0);
    }
}

Expression::Expression(Node* n_c){
    Call* c = dynamic_cast<Call*>(n_c);
    string rule = "Call";
    this->call = c;
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
    this->num = num;
}

Expression::Expression(Node* n_type, Node* n_exp){
    Type* type = dynamic_cast<Type*>(n_type);
    Expression* exp = dynamic_cast<Expression*>(n_exp);
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
