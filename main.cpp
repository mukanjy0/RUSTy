#include <iostream>
#include "Scanner.h"

using namespace std;

int main(int argc, char* argv[]) {
    char* filename = argv[1];

    Scanner scanner (filename);

    while (!scanner.eof()) {
        cout << scanner.getNextToken() << endl;
    }

    return 0;
}
