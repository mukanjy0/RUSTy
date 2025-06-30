#include "Scanner.h"
#include "Fun.h"

class Parser {
private:
    Scanner* scanner;

    Token::Type peek();
    bool check(Token::Type type);
    bool match(Token::Type type);
    Token currentToken();

    BinaryExp::Operation tokenTypeToBinaryOperation(Token::Type type);

    Block* parseBlock();
    Param parseParameter();
    std::pair<std::string, Fun*> parseFunction();
    Stmt* parseStatement();
    Exp* parseExpression();
    Exp* parseNotExp();
    Exp* parseRelationalExp();
    Exp* parseArithmeticExp();
    Exp* parseTermExp();
    Exp* parseFactorExp();

public:
    Parser(char* filename);
    ~Parser();

    Program* parse();
};
