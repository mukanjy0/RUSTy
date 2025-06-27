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
