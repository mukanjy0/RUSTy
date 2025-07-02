#ifndef PARSER_H
#define PARSER_H

#include "../lexic/Scanner.h"
#include "Fun.h"

class Parser {
private:
    Scanner* scanner;

    Token::Type peek();
    bool check(Token::Type type);
    bool match(Token::Type type);
    Token currentToken();

    static std::string debugInfo(const Token& token);
    void ensureSemicolon(const std::string& message);
    BinaryExp::Operation tokenTypeToBinaryOperation(Token::Type type);

    Block* parseBlock();
    Param parseParameter();
    std::pair<std::string, Fun*> parseFunction();
    Exp* parseRhs();
    Stmt* parseStatement();
    Exp* parseExpression();
    Exp* parseNotExp();
    Exp* parseRelationalExp();
    Exp* parseArithmeticExp();
    Exp* parseTermExp();
    Exp* parseReferenceFactorExp();
    Exp* parseFactorExp();

public:
    explicit Parser(char* filename);
    ~Parser();

    Program* parse();
};

#endif
