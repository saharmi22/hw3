#include <vector>
#include "SymbolTable.hpp"
#include "Symbol.hpp"
using namespace std;

class TableStack{
    private:
        vector<SymbolTable> table_stack;
        vector<int> offset_stack;

    public:
        TableStack();

        ~TableStack();

        void addTableToStack();

        SymbolTable getLastTable();

        void popTableFromStack();

        void addSymbol(string name, string type, bool is_func, vector<Symbol> params);

        bool symbolExists(string name);

        Symbol* getSymbol(string name);
};