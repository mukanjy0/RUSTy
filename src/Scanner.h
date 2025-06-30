#include "Token.h"

class Scanner {
private:
    char* source;
    int size;
    int pos {};
    Token current {};

    bool isWhitespace (char ch);
    void advance ();
public:
    Scanner(char* filename);
    ~Scanner();

    bool eof ();
    Token::Type peek();
    bool check (const Token& token);
    bool match (const Token& token);
    Token getToken ();
    Token getNextToken ();
};
