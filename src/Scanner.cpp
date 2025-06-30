#include "Scanner.h"
#include <cstring>
#include <fstream>
#include <sstream>
#include <error.h>

using namespace std;

int Scanner::increasePos() {
    ++col;
    return ++pos;
}

bool Scanner::isWhitespace(char ch) {
    if (ch == '\n') {
        ++line;
        col = 0;
    }
    return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
}

void Scanner::advance() {
    while (pos < size && isWhitespace(source[pos])) {
        increasePos();
    }
    if (pos == size) current.type = Token::END;
    if (current.type == Token::END) return;

    current.line = line;
    current.col = col;

    switch (source[pos]) {
        case ':': current.type = Token::COLON; break;
        case ';': current.type = Token::SEMICOLON; break;
        case ',': current.type = Token::COMMA; break;
        case '.': 
            if (pos + 1 < size && source[pos + 1] == '.') {
                increasePos();
                if (pos + 1 < size && source[pos + 1] == '=') {
                    increasePos();
                    current.type = Token::RANGE_IN; 
                }
                else {
                    current.type = Token::RANGE_EX; 
                }
            }
            else {
                current.type = Token::DOT; 
            }
            break;
        case '\\': current.type = Token::BACKWARD_SLASH; break;
        case '{': current.type = Token::OPEN_CURLY; break;
        case '}': current.type = Token::CLOSE_CURLY; break;
        case '[': current.type = Token::OPEN_BRACKET; break;
        case ']': current.type = Token::CLOSE_BRACKET; break;
        case '(': current.type = Token::OPEN_PARENTHESIS; break;
        case ')': current.type = Token::CLOSE_PARENTHESIS; break;
        case '\'': 
            if (pos + 1 < size && source[pos + 1] == '\'') {
                throw runtime_error("empty character literal");
            }
            // standard characters like 'a'
            if (pos + 2 < size && source[pos + 2] == '\'') {
                current.content = string(1, source[pos + 1]);
                pos += 2;
                current.type = Token::CHAR; 
            }
            // escaped characters like '\n'
            else if (pos + 3 < size && source[pos + 3] == '\''
                && source[pos + 1] == '\\'
                ) {
                current.content = string(1, source[pos + 1]) + string(1, source[pos+2]);
                pos += 3;
                current.type = Token::CHAR; 
            }
            else {
                current.type = Token::SINGULAR_QUOTE; 
            }
            break;
        case '&': 
            if (source[increasePos()] == '&') current.type = Token::LAND;
            else {
                throw std::runtime_error("Invalid operator: &" + string(1, source[pos]));
            }
            break;
        case '|': 
            if (source[increasePos()] == '|') current.type = Token::LOR;
            else {
                throw std::runtime_error("Invalid operator: |" + string(1, source[pos]));
            }
            break;
        case '=': 
            if (source[pos + 1] == '=') {
                increasePos();
                current.type = Token::EQ;
            } 
            else {
                current.type = Token::ASSIGN;
            }
            break;
        case '>': 
            if (source[pos + 1] == '=') {
                increasePos();
                current.type = Token::GE;
            }
            else {
                current.type = Token::GT;
            }
            break;
        case '<': 
            if (source[pos + 1] == '=') {
                increasePos();
                current.type = Token::LE;
            }
            else {
                current.type = Token::LT;
            }
            break;
        case '!': 
            if (source[pos + 1] == '=') {
                increasePos();
                current.type = Token::NEQ;
            }
            else {
                current.type = Token::LNOT;
            }
            break;
        case '+': current.type = Token::PLUS; break;
        case '-': current.type = Token::MINUS; break;
        case '*': current.type = Token::TIMES; break;
        case '/': 
            if (pos + 1 == size || source[pos + 1] != '/') {
                current.type = Token::DIV;
                break;
            }
            increasePos();
            while (pos + 1 < size && source[increasePos()] != '\n') {}
            return advance();
        case '"': 
            while (increasePos() < size && source[pos] != '"') {
                current.content += string(1, source[pos]);
            }
            if (pos == size) {
                throw std::runtime_error("Opening double quotations where not matched with closing ones");
            }
            current.type = Token::STRING;
            break;
        default:
            if (isalpha(source[pos])) {
                current.content = string(1, source[pos]);
                while (pos + 1 < size && isalnum(source[pos + 1])) {
                    increasePos();
                    current.content += source[pos];
                }
                if (current.content == "i32"
                    || current.content == "bool"
                    || current.content == "char"
                ) {
                    current.type = Token::TYPE;
                }
                else if (current.content == "true"
                    || current.content == "false"
                ) {
                    current.type = Token::BOOLEAN;
                }
                else if (current.content == "fn") {
                    current.type = Token::FN;
                }
                else if (current.content == "return") {
                    current.type = Token::RETURN;
                }
                else if (current.content == "break") {
                    current.type = Token::BREAK;
                }
                else if (current.content == "let") {
                    current.type = Token::LET;
                }
                else if (current.content == "mut") {
                    current.type = Token::MUT;
                }
                else if (current.content == "for") {
                    current.type = Token::FOR;
                }
                else if (current.content == "in") {
                    current.type = Token::IN;
                }
                else if (current.content == "while") {
                    current.type = Token::WHILE;
                }
                else if (current.content == "loop") {
                    current.type = Token::LOOP;
                }
                else if (current.content == "if") {
                    current.type = Token::IF;
                }
                else if (current.content == "else") {
                    current.type = Token::ELSE;
                }
                else if (current.content == "println"
                        && pos < size && source[pos + 1] == '!') 
                {
                    increasePos();
                    current.type = Token::PRINT;
                }
                else {
                    current.type = Token::ID;
                }
            }
            else if (isdigit(source[pos])) {
                current.content = string(1, source[pos]);
                while (pos + 1 < size && isdigit(source[pos + 1])) {
                    increasePos();
                    current.content += source[pos];
                }
                current.type = Token::NUMBER;
            }
            else {
                throw std::runtime_error("Invalid token");
            }
    }
    increasePos();
}

Scanner::Scanner (char* filename) {
    ifstream f (filename);

    stringstream buffer;
    buffer << f.rdbuf();

    size = buffer.str().length();
    source = new char [size];
    strcpy (source, buffer.str().c_str());

    f.close();
}

Scanner::~Scanner() {
    delete source;
}

bool Scanner::eof () {
    return current.type == Token::END;
}

Token::Type Scanner::peek() {
    int prevPos = pos;
    Token aux = current;

    advance();

    swap(aux, current);
    pos = prevPos;

    return aux.type;
}

bool Scanner::check(const Token& token) {
    return current.type == token.type;
}

bool Scanner::match(const Token& token) {
    if (current.type != token.type) return false;
    advance();
    return true;
}

void Scanner::next () { 
    advance();
}

Token Scanner::getToken () { return current; }

Token Scanner::getNextToken () { 
    advance();
    return current; 
}
