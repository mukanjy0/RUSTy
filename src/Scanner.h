#ifndef SCANNER_H
#define SCANNER_H

#include "Token.h"

class Scanner {
private:
    char* source;
    int size;
    int pos {};
    int col {1};
    int line {1};
    Token current {};

    int increasePos ();
    bool isWhitespace (char ch);
    void advance ();
public:
    Scanner(char* filename);
    ~Scanner();

    bool eof ();
    Token::Type peek();
    bool check (const Token& token);
    bool match (const Token& token);
    void next ();
    Token getToken ();
    Token getNextToken ();
};

#endif
