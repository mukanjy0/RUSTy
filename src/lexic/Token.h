#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

class Scanner;

class Token {
public:
    enum Type {
        BEG, END, ID, RANGE_IN, RANGE_EX, REFERENCE, ARROW, // special tokens
        FN, RETURN, BREAK, LET, MUT, FOR, IN, WHILE, LOOP, IF, ELSE, // reserved keywords
        SEMICOLON, COLON, COMMA, DOT, BACKWARD_SLASH,
        OPEN_CURLY, CLOSE_CURLY, OPEN_BRACKET, CLOSE_BRACKET, OPEN_PARENTHESIS, CLOSE_PARENTHESIS,
        SINGULAR_QUOTE, // symbols
        ASSIGN, PLUS_ASSIGN, MINUS_ASSIGN, TIMES_ASSIGN, DIV_ASSIGN, // assignment
        LAND, LOR, LNOT, // logical operators
        EQ, NEQ, LT, GT, LE, GE,  // relational operators
        PLUS, MINUS, TIMES, DIV, // arithmetic operators
        TYPE, BOOLEAN, NUMBER, STRING, CHAR, // groups
        PRINT // macros
    };

    Token();
    explicit Token(Type type);
    Token(Type type, std::string content, int col, int line);
    ~Token();
    Type getType() const;
    std::string getContent() const;
    operator std::string() const;

private:
    friend class Scanner;
    friend class Parser;

    Type type;
    std::string content;
    int col{};
    int line{};
    friend std::ostream& operator<<(std::ostream& out, const Token& token);
};

#endif
