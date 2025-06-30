#include <iostream>
#include "src/Parser.h"
#include "src/Visitor.h"

using namespace std;

int main(int argc, char* argv[]) {
    // input errors
    if (argc != 2) {
        cerr << "Incorrect number of arguments" << endl << "Usage: " << argv[0] << " <input_file>" << endl;
        exit(1);
    }
    char* filename = argv[1];

    Scanner scanner (filename);

    while (!scanner.eof()) {
        cout << scanner.getNextToken() << endl;
    }

    Parser parser (filename);

    Program* program = parser.parse();

    Printer printer;

    printer.visit(program);

    return 0;
}
