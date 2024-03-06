#include <vector>
#include "Symbol.hpp"
using namespace std;

class SymbolTable{
    private:
        vector<Symbol> table;
        int curr_offset;

    public:
        SymbolTable(int init_offset);

        void addSymbol(string name, string type, bool is_func, vector<Symbol> params);

        bool symbolExists(string name);

        Symbol* getSymbol(string name);

        // void removeSymbol(string name);

        ~SymbolTable(){};

};