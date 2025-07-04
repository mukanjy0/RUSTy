#include "Token.h"
#include <iostream>

Token::Token() : type(BEG) {}

Token::Token(Type type) : type(type) {}

Token::Token(Type type, std::string content, int col, int line)
    : type(type), content(content), col(col), line(line) {}

Token::~Token() {}

Token::Type Token::getType() const {
    return type;
}

std::string Token::getContent() const {
    return content;
}

Token::operator std::string() const {
    switch (type) {
        case Token::BEG: return "BEG";
        case Token::END: return "END";
        case Token::ID: return "ID";
        case Token::RANGE_IN: return "RANGE_IN";
        case Token::RANGE_EX: return "RANGE_EX";
        case Token::REFERENCE: return "REFERENCE";
        case Token::ARROW: return "ARROW";
        case Token::FN: return "FN";
        case Token::RETURN: return "RETURN";
        case Token::BREAK: return "BREAK";
        case Token::LET: return "LET";
        case Token::MUT: return "MUT";
        case Token::FOR: return "FOR";
        case Token::IN: return "IN";
        case Token::WHILE: return "WHILE";
        case Token::LOOP: return "LOOP";
        case Token::IF: return "IF";
        case Token::ELSE: return "ELSE";
        case Token::SEMICOLON: return "SEMICOLON";
        case Token::COLON: return "COLON";
        case Token::COMMA: return "COMMA";
        case Token::DOT: return "DOT";
        case Token::BACKWARD_SLASH: return "BACKWARD_SLASH";
        case Token::OPEN_CURLY: return "OPEN_CURLY";
        case Token::CLOSE_CURLY: return "CLOSE_CURLY";
        case Token::OPEN_BRACKET: return "OPEN_BRACKET";
        case Token::CLOSE_BRACKET: return "CLOSE_BRACKET";
        case Token::OPEN_PARENTHESIS: return "OPEN_PARENTHESIS";
        case Token::CLOSE_PARENTHESIS: return "CLOSE_PARENTHESIS";
        case Token::SINGULAR_QUOTE: return "SINGULAR_QUOTE";
        case Token::ASSIGN: return "ASSIGN";
        case Token::PLUS_ASSIGN: return "PLUS_ASSIGN";
        case Token::MINUS_ASSIGN: return "MINUS_ASSIGN";
        case Token::TIMES_ASSIGN: return "TIMES_ASSIGN";
        case Token::DIV_ASSIGN: return "DIV_ASSIGN";
        case Token::LAND: return "LAND";
        case Token::LOR: return "LOR";
        case Token::LNOT: return "LNOT";
        case Token::EQ: return "EQ";
        case Token::NEQ: return "NEQ";
        case Token::LT: return "LT";
        case Token::GT: return "GT";
        case Token::LE: return "LE";
        case Token::GE: return "GE";
        case Token::PLUS: return "PLUS";
        case Token::MINUS: return "MINUS";
        case Token::TIMES: return "TIMES";
        case Token::DIV: return "DIV";
        case Token::TYPE: return "TYPE";
        case Token::BOOLEAN: return "BOOLEAN";
        case Token::NUMBER: return "NUMBER";
        case Token::CHAR: return "CHAR";
        case Token::STRING: return "STRING";
        case Token::PRINT: return "PRINT";
    }
    return {};
}

std::ostream& operator<<(std::ostream& out, const Token& token) {
    out << std::string(token);
    return out;
}
