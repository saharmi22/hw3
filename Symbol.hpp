#include <string>
#include <vector>
using namespace std;

class Symbol
{
private:
    string type;
    string name;
    int offset;
    bool func;
    vector<Symbol> params;

public:
    Symbol(string type, string name, int offset, bool is_func, vector<Symbol> params);
    string getName();
    string getType();
    bool isFunc();
    vector<Symbol> getFuncParams();
    ~Symbol(){};
};
