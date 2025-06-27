#include <iostream>
#include <string>

class Scanner;

class Token {
    friend Scanner;
private:
    enum Type {
        BEG, END, ID, // special tokens
        FN, RETURN, LET, MUT, FOR, IN, WHILE, LOOP, IF, ELSE, // reserved keywords
        SEMICOLON, COLON, COMMA, DOT, BACKWARD_SLASH,
        OPEN_CURLY, CLOSE_CURLY, OPEN_BRACKET, CLOSE_BRACKET, OPEN_PARENTHESIS, CLOSE_PARENTHESIS,
        SINGULAR_QUOTE, ASSIGN, // symbols
        LAND, LOR, NOT, // logical operators
        EQ, NEQ, LT, GT, LE, GE,  // relational operators
        PLUS, MINUS, TIMES, DIV, // arithmetic operators
        TYPE, BOOLEAN, NUMBER, STRING, CHAR, // groups
        PRINT // macros
    };

    Type type;
    std::string content;
public:
    Token();
    Token(Type type);
    Token(Type type, std::string content);
    ~Token();
    Type getType() const;
    std::string getContent() const;

    friend std::ostream& operator<<(std::ostream& out, const Token& token) {
        switch (token.getType()) {
            case BEG: out << "BEG"; break;
            case END: out << "END"; break;
            case ID: out << "ID"; break;
            case FN: out << "FN"; break;
            case RETURN: out << "RETURN"; break;
            case LET: out << "LET"; break;
            case MUT: out << "MUT"; break;
            case FOR: out << "FOR"; break;
            case IN: out << "IN"; break;
            case WHILE: out << "WHILE"; break;
            case LOOP: out << "LOOP"; break;
            case IF: out << "IF"; break;
            case ELSE: out << "ELSE"; break;
            case SEMICOLON: out << "SEMICOLON"; break;
            case COLON: out << "COLON"; break;
            case COMMA: out << "COMMA"; break;
            case DOT: out << "DOT"; break;
            case BACKWARD_SLASH: out << "BACKWARD_SLASH"; break;
            case OPEN_CURLY: out << "OPEN_CURLY"; break;
            case CLOSE_CURLY: out << "CLOSE_CURLY"; break;
            case OPEN_BRACKET: out << "OPEN_BRACKET"; break;
            case CLOSE_BRACKET: out << "CLOSE_BRACKET"; break;
            case OPEN_PARENTHESIS: out << "OPEN_PARENTHESIS"; break;
            case CLOSE_PARENTHESIS: out << "CLOSE_PARENTHESIS"; break;
            case SINGULAR_QUOTE: out << "SINGULAR_QUOTE"; break;
            case ASSIGN: out << "ASSIGN"; break;
            case LAND: out << "LAND"; break;
            case LOR: out << "LOR"; break;
            case NOT: out << "NOT"; break;
            case EQ: out << "EQ"; break;
            case NEQ: out << "NEQ"; break;
            case LT: out << "LT"; break;
            case GT: out << "GT"; break;
            case LE: out << "LE"; break;
            case GE: out << "GE"; break;
            case PLUS: out << "PLUS"; break;
            case MINUS: out << "MINUS"; break;
            case TIMES: out << "TIMES"; break;
            case DIV: out << "DIV"; break;
            case TYPE: out << "TYPE"; break;
            case BOOLEAN: out << "BOOLEAN"; break;
            case NUMBER: out << "NUMBER"; break;
            case CHAR: out << "CHAR"; break;
            case STRING: out << "STRING"; break;
            case PRINT: out << "PRINT"; break;
        }

        return out;
    }
};
