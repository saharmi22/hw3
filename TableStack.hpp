#include <vector>
#include "SymbolTable.hpp"
#include "hw3_output.hpp"

using namespace std;

class TableStack{
    private:
        vector<SymbolTable> table_stack;
        vector<int> offset_stack;

    public:
        TableStack();

        ~TableStack(){};

        void addTableToStack();

        SymbolTable getLastTable();

        void popTableFromStack();

        void addSymbol(string name, string type, bool is_func, vector<Symbol> params);

        void printScope();

        bool symbolExists(string name);

        Symbol* getSymbol(string name);
};