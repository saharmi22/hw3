#include <string>
#include "TableStack.hpp"
using namespace std;


class Node{  
    string name;

    public:
        Node(string n);  
        Node();
        string getName();
};

class Program : Node{
    Statements* s;

    public:
        Program(Statements* s);
};


class Statements : Node{
    Statements* statements;
    Statement* statement;


    public:
        Statements(Statement* s1);
        Statements(Statement* s2, Statements* s1);
};


class Statement : Node{
    Statements* statements;
    Type* type;
    Node* node;
    Expression* exp;
    Call* call;
    Statement* statement1;
    Statement* statement2;
    
    public:
        bool is_loop_statment;
        bool is_in_loop;
        string loop_statement;
        Statement(string type);
        Statement(Statements* s);
        Statement(Type* t, Node* id);
        Statement(Type* t, Node* id, Expression* e);
        Statement(Node* id, Expression* e);
        Statement(Call* c);
        Statement(Expression* e, Statement* s, bool is_if);
        Statement(Expression* e, Statement* s1, Statement* s2);
        bool checkLoopStatement();

};


class Call : Node{
    Node* id;
    Expression* exp;
    string ret_type;
    
    public:
        Call(Node *id, Expression *exp);
};

class Type : Node{
    string type;

    public:
        Type(string type);
        string getType();

};

class Expression : Node{
    Expression* exp1;
    Expression* exp2;
    Node* id;
    int num;
    Call* call;
    Type* type;
    string string_true_false;
    string exp_type;
    bool is_in_loop;

    public:
        Expression(Expression* exp,  bool is_parens);
        Expression(Expression* exp1, Expression* exp2, string middle_word);
        Expression(Node* id);
        Expression(Call* c);
        Expression(int num, bool is_byte);
        //byte num
        Expression(Type* type);
        Expression(Type* type, Expression* exp);
        Expression(string string_true_false);
        string getExpType();
};