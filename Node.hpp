#include <string>
#include "TableStack.hpp"
using namespace std;

class Call;
class Program;
class Statement;
class Statements;
class Expression;
class Type;
class Node;

class Node{  
    string name;

    public:
        Node(string n);  
        Node();
        ~Node() = default;
        string getName();
};

class Program : public Node{
    Statements* s;

    public:
        Program(Node* s);
        /*~Program(){
            delete s;
        }*/
};


class Statements : public Node{
    Statements* statements;
    Statement* statement;


    public:
        Statements(Node* s1);
        Statements(Node* s2, Node* s1);
        /*~Statements(){
            delete statement;
            if (statements){
                delete statements;
            }
        }*/
};


class Statement : public Node{
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
        Statement(Node* s);
        Statement(Node* t_e, Node* id);
        Statement(Node* t, Node* id, Node* e);
        Statement(Node* e, Node* s, bool is_if);
        bool checkLoopStatement();
        /*~Statement(){
            if (statements){
                delete statements;
            }
            if (type){
                delete type;
            }
            if (node){
                delete node;
            }
            if (exp){
                delete exp;
            }
            if (call){
                delete call;
            }
            if (statement1){
                delete statement1;
            }
            if (statement2){
                delete statement2;
            }
        }*/

};


class Call : public Node{
    Node* id;
    Expression* exp;
    string ret_type;
    
    public:
        Call(Node *id, Node *exp);
        /*~Call(){
            delete id;
            delete exp;
        }*/
};

class Type : public Node{
    string type;

    public:
        Type(string type);
        string getType();
        /*~Type(){};*/

};

class Expression : public Node{
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
        Expression(Node* n_exp,  bool is_parens);
        Expression(Node* n_exp1, Node* n_exp2, string middle_word);
        Expression(Node* n_c);
        Expression(int num, bool is_byte);
        //byte num
        Expression(Node* n_type, Node* n_exp);
        Expression(string string_true_false);
        string getExpType();
        /*~Expression(){
            if (exp1){
                delete exp1;
            }
            if (exp2){
                delete exp2;
            }
            if (id){
                delete id;
            }
            if (call){
                delete call;
            }
            if (type){
                delete type;
            }
        }*/
};