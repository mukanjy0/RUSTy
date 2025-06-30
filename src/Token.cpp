#include "Token.h"
#include <iostream>

Token::Token() : type(BEG) {

}

Token::Token(Type type) : type(type) {

}

Token::Token(Type type, std::string content) : type(type), content(content) {

}

Token::~Token() {

}

Token::Type Token::getType() const {
    return type;
}

std::string Token::getContent() const {
    return content;
}

std::ostream& operator<<(std::ostream& out, const Token& token) {
    switch (token.getType()) {
        case Token::BEG: out << "BEG"; break;
        case Token::END: out << "END"; break;
        case Token::ID: out << "ID"; break;
        case Token::RANGE_IN: out << "RANGE_IN"; break;
        case Token::RANGE_EX: out << "RANGE_EX"; break;
        case Token::FN: out << "FN"; break;
        case Token::RETURN: out << "RETURN"; break;
        case Token::BREAK: out << "BREAK"; break;
        case Token::LET: out << "LET"; break;
        case Token::MUT: out << "MUT"; break;
        case Token::FOR: out << "FOR"; break;
        case Token::IN: out << "IN"; break;
        case Token::WHILE: out << "WHILE"; break;
        case Token::LOOP: out << "LOOP"; break;
        case Token::IF: out << "IF"; break;
        case Token::ELSE: out << "ELSE"; break;
        case Token::SEMICOLON: out << "SEMICOLON"; break;
        case Token::COLON: out << "COLON"; break;
        case Token::COMMA: out << "COMMA"; break;
        case Token::DOT: out << "DOT"; break;
        case Token::BACKWARD_SLASH: out << "BACKWARD_SLASH"; break;
        case Token::OPEN_CURLY: out << "OPEN_CURLY"; break;
        case Token::CLOSE_CURLY: out << "CLOSE_CURLY"; break;
        case Token::OPEN_BRACKET: out << "OPEN_BRACKET"; break;
        case Token::CLOSE_BRACKET: out << "CLOSE_BRACKET"; break;
        case Token::OPEN_PARENTHESIS: out << "OPEN_PARENTHESIS"; break;
        case Token::CLOSE_PARENTHESIS: out << "CLOSE_PARENTHESIS"; break;
        case Token::SINGULAR_QUOTE: out << "SINGULAR_QUOTE"; break;
        case Token::ASSIGN: out << "ASSIGN"; break;
        case Token::LAND: out << "LAND"; break;
        case Token::LOR: out << "LOR"; break;
        case Token::LNOT: out << "LNOT"; break;
        case Token::EQ: out << "EQ"; break;
        case Token::NEQ: out << "NEQ"; break;
        case Token::LT: out << "LT"; break;
        case Token::GT: out << "GT"; break;
        case Token::LE: out << "LE"; break;
        case Token::GE: out << "GE"; break;
        case Token::PLUS: out << "PLUS"; break;
        case Token::MINUS: out << "MINUS"; break;
        case Token::TIMES: out << "TIMES"; break;
        case Token::DIV: out << "DIV"; break;
        case Token::TYPE: out << "TYPE"; break;
        case Token::BOOLEAN: out << "BOOLEAN"; break;
        case Token::NUMBER: out << "NUMBER"; break;
        case Token::CHAR: out << "CHAR"; break;
        case Token::STRING: out << "STRING"; break;
        case Token::PRINT: out << "PRINT"; break;
    }

    return out;
}
