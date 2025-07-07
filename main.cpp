#include <iostream>
#include <fstream>
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

    cout << "\n=======================\n";
    cout << "Printing scanned tokens...";
    cout << "\n=======================\n";

    while (!scanner.eof()) {
        cout << scanner.getNextToken() << " - " << scanner.getTokenContent() << endl;
    }

    Parser parser (filename);
    Program* program = parser.parse();

    Printer printer;

    cout << "\n=======================\n";
    cout << "Printing source code...";
    cout << "\n=======================\n";

    printer.visit(program);

    SymbolTable table;
    NameRes nameRes(&table);
    nameRes.visit(program);

    TypeCheck typeCheck(&table);
    typeCheck.visit(program);

    std::ofstream f ("a.s");
    CodeGen codeGen(&table, f);
    codeGen.visit(program);
    f.close();

    return 0;
}
