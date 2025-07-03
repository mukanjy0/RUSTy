#include <iostream>
#include "src/syntactic/Parser.h"
#include "src/semantic/Printer.h"
#include "src/semantic/NameRes.h"
#include "src/semantic/TypeCheck.h"
#include "src/semantic/CodeGen.h"
#include "src/semantic/SymbolTable.h"

using namespace std;

int main(const int argc, char* argv[]) {
    // input errors
    if (argc != 2) {
        cerr << "Incorrect number of arguments" << endl << "Usage: " << argv[0] << " <input_file>" << endl;
        exit(1);
    }
    char* filename = argv[1];

    Scanner scanner (filename);

    while (!scanner.eof()) {
        cout << scanner.getNextToken() << "- (" << scanner.getTokenContent() << ")"<< endl;
    }

    Parser parser (filename);
    Program* program = parser.parse();

    Printer printer;
    printer.visit(program);

    SymbolTable table;
    NameRes nameRes(&table);
    nameRes.visit(program);

    TypeCheck typeCheck(&table);
    typeCheck.visit(program);

    CodeGen codeGen(&table, std::cout);
    codeGen.visit(program);

    return 0;
}
