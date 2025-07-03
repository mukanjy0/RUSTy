#ifndef SCANNER_H
#define SCANNER_H

#include "Token.h"

class Scanner {
private:
    // snapshot of the configuration of Scanner at one instant
    class Snapshot {
    private:
        friend class Scanner;

        int pos;
        int line;
        int col;
        Token current;
    public:
      Snapshot(int pos, int line, int col, const Token& current)
          : pos(pos), line(line), col(col), current(current) {}
    };

    char* source;
    int size;
    int pos {};
    int line {1};
    int col {1};
    Token current {};

    int increasePos ();
    int increasePos (const int& rhs);
    bool isWhitespace (char ch);
    void advance ();
public:
    explicit Scanner(char* filename);
    ~Scanner();

    bool eof ();
    Token::Type peek();
    bool check (const Token& token);
    bool match (const Token& token);
    void next ();
    Token getToken ();
    std::string getTokenContent ();
    Token getNextToken ();
    Snapshot getSnapshot ();
    void restoreSnapshot (const Snapshot& snapshot);
};

#endif
