#include <iostream>
#include "src/syntax/Parser.h"
#include "src/semantic/Printer.h"

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

    return 0;
}
