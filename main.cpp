#include <iostream>
#include "src/syntactic/Parser.h"
#include "src/semantic/Printer.h"

using namespace std;

int main(int argc, char* argv[]) {
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

    return 0;
}
