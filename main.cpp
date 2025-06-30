#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "src/Scanner.h"

using namespace std;

void dummyAssembly(ofstream& outfile) {
    // Just a dummy assembly code for demonstration purposes
    outfile << R"(.data
print_fmt: .string "%ld \n"
.text
 .globl main
main:
 pushq %rbp
 movq %rsp, %rbp
while_0:
 movq $1, %rax
 cmpq $0, %rax
 je endwhile_0
 movq $3, %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 jmp endwhile_0
 jmp while_0
endwhile_0:
 movq $0, %rax
 jmp .end_main
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
    )" << endl;
}

int main(int argc, char* argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 2) {
        cerr << "Incorrect number of arguments" << endl << "Usage: " << argv[0] << " <input_file>" << endl;
        exit(1);
    }
    char* filename = argv[1];
    string inputFile(argv[1]);
    filesystem::path inputPath(inputFile);
    string baseName = inputPath.stem().string();
    filesystem::path outputPath = std::filesystem::path("input/ass") / (baseName + ".s");
    string outputFilename = outputPath.string();
    filesystem::create_directories("input/ass");
    ofstream outfile(outputFilename);
    if (!outfile.is_open()) {
        cerr << "Error al crear el archivo de salida: " << outputFilename << endl;
        return 1;
    }
    dummyAssembly(outfile);
//  Scanner scanner (filename);
//  while (!scanner.eof()) {
//      cout << scanner.getNextToken() << endl;
//  }

    outfile.close();


    return 0;
}
