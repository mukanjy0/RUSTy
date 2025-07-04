#include "Parser.h"

Parser::Parser(char* filename) {
    scanner = new Scanner(filename);
    match(Token::BEG);
}
Parser::~Parser() {
    delete scanner;
}

std::pair<int,int> Parser::getPos() {
    return {currentToken().line, currentToken().col};
}

std::string Parser::debugInfo(const Token& token) {
    return "Token: " + std::string(token) + " | Content: " + token.content
             + "\nat line: " + std::to_string(token.line)
             + " | column: " + std::to_string(token.col);
}

void Parser::ensureSemicolon(const std::string& message) {
    if (!match(Token::SEMICOLON)) {
        throw std::runtime_error(message
                                 + std::string("\ngot: ")
                                 + debugInfo(currentToken()));
    }
}

BinaryExp::Operation Parser::tokenTypeToBinaryOperation(Token::Type type) {
    switch(type) {
        case Token::LAND: return BinaryExp::LAND;
        case Token::LOR: return BinaryExp::LOR;
        case Token::EQ: return BinaryExp::EQ;
        case Token::NEQ: return BinaryExp::NEQ;
        case Token::LT: return BinaryExp::LT;
        case Token::GT: return BinaryExp::GT;
        case Token::LE: return BinaryExp::LE;
        case Token::GE: return BinaryExp::GE;
        case Token::PLUS: return BinaryExp::PLUS;
        case Token::MINUS: return BinaryExp::MINUS;
        case Token::TIMES: return BinaryExp::TIMES;
        case Token::DIV: return BinaryExp::DIV;
        case Token::PLUS_ASSIGN: return BinaryExp::PLUS;
        case Token::MINUS_ASSIGN: return BinaryExp::MINUS;
        case Token::TIMES_ASSIGN: return BinaryExp::TIMES;
        case Token::DIV_ASSIGN: return BinaryExp::DIV;
        default:
            throw std::runtime_error("expected binary operation token\ngot: "
                                     + debugInfo(currentToken()));
    }
}

Token::Type Parser::peek() {
    return scanner->peek();
}
bool Parser::check(Token::Type type) {
    return scanner->check(Token{type});
}
bool Parser::match(Token::Type type) {
    return scanner->match(Token{type});
}
Token Parser::currentToken() {
    return scanner->getToken();
}

Program* Parser::parse() {
    std::list<std::pair<std::string,Fun*>> funs;
    auto [id, fun] = parseFunction();
    funs.emplace_back(id, fun);
    while (!scanner->eof()) {
        auto [id, fun] = parseFunction();
        funs.emplace_back(id, fun);
    }
    return new Program(funs);
}

Block* Parser::parseBlock() {
    auto [line, col] = getPos();
    if (!match(Token::OPEN_CURLY)) {
        throw std::runtime_error("expected '{' to start block\ngot: "
                                 + debugInfo(currentToken()));
    }

    std::list<Stmt*> stmts;
    Stmt* stmt;
    while (!check(Token::CLOSE_CURLY)) {
        stmt = parseStatement();
        stmts.push_back(stmt);
    }

    if (!match(Token::CLOSE_CURLY)) {
        throw std::runtime_error("expected '}' to end block\ngot: "
                                 + debugInfo(currentToken()));
    }

    return new Block(line, col, stmts);
}

Param Parser::parseParameter() {
    auto [line, col] = getPos();

    Param param;
    param.line = line;
    param.col = col;
    if (!check(Token::ID)) {
        throw std::runtime_error("expected id in parameter list\ngot: "
                                 + debugInfo(currentToken()));
    }
    param.id = currentToken().content;
    match(Token::ID);

    if (!match(Token::COLON)) {
        throw std::runtime_error("expected ':' in parameter list\ngot: "
                                 + debugInfo(currentToken()));
    }

    if (!check(Token::TYPE)) {
        throw std::runtime_error("expected type in parameter list\ngot: "
                                 + debugInfo(currentToken()));
    }
    param.type = Value::stringToType(currentToken().content);
    match(Token::TYPE);

    return param;
}

std::pair<std::string, Fun*> Parser::parseFunction() {
    auto [line, col] = getPos();
    if (!match(Token::FN)) {
        throw std::runtime_error("expected function declaration\ngot: "
                                 + debugInfo(currentToken()));
    }
    if (!check(Token::ID)) {
        throw std::runtime_error("expected function name after 'fn'\ngot: "
                                 + debugInfo(currentToken()));
    }
    std::string id = currentToken().content;
    match(Token::ID);

    if (!match(Token::OPEN_PARENTHESIS)) {
        throw std::runtime_error("expected parenthesis in function declaration\ngot: "
                                 + debugInfo(currentToken()));
    }
    std::list<Param> params;
    if (!check(Token::CLOSE_PARENTHESIS)) {
        Param param = parseParameter();
        params.push_back(param);
        while (match(Token::COMMA)) {
            param = parseParameter();
            params.push_back(param);
        }
    }
    if (!match(Token::CLOSE_PARENTHESIS)) {
        throw std::runtime_error("expected closing parenthesis after parameter list in function declaration\ngot: "
                                 + debugInfo(currentToken()));
    }

    Value::Type type = Value::UNDEFINED;
    if (match(Token::ARROW)) {
        if (!check(Token::TYPE)) {
            throw std::runtime_error("expected type after arrow in function declaration\ngot: "
                                     + debugInfo(currentToken()));
        }
        type = Value::stringToType(currentToken().content);
        match(Token::TYPE);
    }

    Block* block = parseBlock();

    return {id, new Fun(line, col, type, params, block)};
}

Exp* Parser::parseRhs() {
    auto [line, col] = getPos();
    if (match(Token::OPEN_BRACKET)) {
        Exp* exp = parseExpression();
        if (match(Token::SEMICOLON)) {
            Exp* size = parseExpression();
            if (!match(Token::CLOSE_BRACKET)) {
                throw std::runtime_error("expected closing bracket in uniform array\ngot: "
                                         + debugInfo(currentToken()));
            }
            return new UniformArrayExp(line, col, exp, size);
        }
        std::list<Exp*> elements = {exp};
        while (match(Token::COMMA)) {
            elements.push_back(parseExpression());
        }
        if (!match(Token::CLOSE_BRACKET)) {
            throw std::runtime_error("expected closing bracket in array\ngot: "
                                     + debugInfo(currentToken()));
        }
        return new ArrayExp(line, col, elements);
    }
    else {
        return parseExpression();
    }
}

Stmt* Parser::parseStatement() {
    auto [line, col] = getPos();
    if (match(Token::LET)) {
        Value var {};
        if (match(Token::MUT)) {
            var.mut = true;
        }
        std::string id = currentToken().content;
        if (!match(Token::ID)) {
            throw std::runtime_error("expected id in variable declaration\ngot: "
                                     + debugInfo(currentToken()));
        }
        if (match(Token::COLON)) {
            if (match(Token::OPEN_BRACKET)) {
                if (match(Token::REFERENCE)) {
                    var.ref = true;
                }
                if (!check(Token::TYPE)) {
                    throw std::runtime_error("expected type after ':' in array declaration\ngot: "
                                             + debugInfo(currentToken()));
                }
                var.type = Value::stringToType(currentToken().content);
                match(Token::TYPE);

                if (!match(Token::SEMICOLON)) {
                    throw std::runtime_error("expected ';' after type in array declaration\ngot: "
                                             + debugInfo(currentToken()));
                }

                if (!check(Token::NUMBER)) {
                    throw std::runtime_error("expected a number for array size in declaration\ngot: "
                                             + debugInfo(currentToken()));
                }
                var.size = stoi(currentToken().content);
                match(Token::NUMBER);

                if (!match(Token::CLOSE_BRACKET)) {
                    throw std::runtime_error("expected '[' after size in array declaration\ngot: "
                                             + debugInfo(currentToken()));
                }
            }
            else {
                if (match(Token::REFERENCE)) {
                    var.ref = true;
                }
                if (!check(Token::TYPE)) {
                    throw std::runtime_error("expected type after ':' in variable declaration\ngot: "
                                             + debugInfo(currentToken()));
                }
                var.type = Value::stringToType(currentToken().content);
                match(Token::TYPE);
            }
        }
        if (match(Token::SEMICOLON)) {
            return new DecStmt(line, col, id, var);
        }
        if (!match(Token::ASSIGN)) {
            throw std::runtime_error("expected '=' in declaration\ngot: "
                                     + debugInfo(currentToken()));
        }
        Exp* rhs;
        if (var.size == 0) {
            rhs = parseExpression();
        }
        else {
            rhs = parseRhs();
        }
        ensureSemicolon("expected ';' after declaration statement");

        return new DecStmt(line, col, id, var, rhs);

    }
    else if (match(Token::FOR)) {
        std::string id = currentToken().content;
        if (!match(Token::ID)) {
            throw std::runtime_error("expected id after 'for'\ngot: "
                                     + debugInfo(currentToken()));
        }
        if (!match(Token::IN)) {
            throw std::runtime_error("expected 'in' after for id\ngot: "
                                     + debugInfo(currentToken()));
        }
        Exp* start = parseExpression();
        bool inclusive {};
        if (match(Token::RANGE_EX)) {}
        else if (match(Token::RANGE_IN)) {
            inclusive = true;
        }
        else {
            throw std::runtime_error("expected '..' or '..=' in for\ngot: "
                                     + debugInfo(currentToken()));
        }
        Exp* end = parseExpression();
        Block* block = parseBlock();

        return new ForStmt(line, col, id, start, end, block, inclusive);
    }
    else if (match(Token::WHILE)) {
        Exp* cond = parseExpression();
        Block* block = parseBlock();

        return new WhileStmt(line, col, cond, block);
    }
    else if (check(Token::IF) || check(Token::LOOP)) {
        return new ExpStmt(line, col, parseFactorExp(), true);
    }
    else if (match(Token::PRINT)) {
        if (!match(Token::OPEN_PARENTHESIS)) {
            throw std::runtime_error("expected '(' after 'println!'\ngot: "
                                     + debugInfo(currentToken()));
        }
        std::string literal = currentToken().content;
        if (!match(Token::STRING)) {
            throw std::runtime_error("expected string literal inside print\ngot: "
                                     + debugInfo(currentToken()));
        }

        std::list<Exp*> args;
        while (match(Token::COMMA)) {
            Exp* arg = parseExpression();
            args.push_back(arg);
        }

        if (!match(Token::CLOSE_PARENTHESIS)) {
            throw std::runtime_error("expected ')' to close 'println!'\ngot: "
                                     + debugInfo(currentToken()));
        }

        ensureSemicolon("expected ';' after print statement");

        return new PrintStmt(line, col, literal, args);
    }
    else if (match(Token::BREAK)) {
        // if break is last statement in block, it can be missing ';'
        if (match(Token::SEMICOLON) || check(Token::CLOSE_CURLY)) {
            return new BreakStmt(line, col);
        }
        Exp* exp = parseExpression();
        // if break is last statement in block, it can be missing ';'
        if (!match(Token::SEMICOLON) && !check(Token::CLOSE_CURLY)) {
            throw std::runtime_error("expected ';' after break expression\ngot: "
                                     + debugInfo(currentToken()));
        }
        return new BreakStmt(line, col, exp);
    }
    else if (match(Token::RETURN)) {
        // if return is last statement in block, it can be missing ';'
        if (match(Token::SEMICOLON) || check(Token::CLOSE_CURLY)) {
            return new ReturnStmt(line, col);
        }
        Exp* exp = parseExpression();
        // if return is last statement in block, it can be missing ';'
        if (!match(Token::SEMICOLON) && !check(Token::CLOSE_CURLY)) {
            throw std::runtime_error("expected ';' after break expression\ngot: "
                                     + debugInfo(currentToken()));
        }

        return new ReturnStmt(line, col, exp);
    }
    else if (match(Token::REFERENCE)) {
        std::string id = currentToken().content;
        if (!match(Token::ID)) {
            throw std::runtime_error("expected id after '&' in statement\ngot: "
                                     + debugInfo(currentToken()));
        }

        Exp* lhs = new Variable(line, col, id);

        if (!match(Token::ASSIGN)) {
            throw std::runtime_error("expected '=' after & id in assignment statement\ngot: "
                                     + debugInfo(currentToken()));
        }
        Exp* rhs = parseRhs();

        ensureSemicolon("expected ';' after assignment statement");

        return new AssignStmt(line, col, lhs, rhs, true);
    }
    else if (check(Token::ID)) {
        // take a snapshot of scanner for possible later restoration
        auto snapshot = scanner->getSnapshot();

        std::string id = currentToken().content;
        match(Token::ID);
        if (match(Token::ASSIGN)) {
            Exp* lhs = new Variable(line, col, id);
            Exp* rhs = parseRhs();

            ensureSemicolon("expected ';' after assignment statement");

            return new AssignStmt(line, col, lhs, rhs);
        }
        else if (
                check(Token::PLUS_ASSIGN)
                || check(Token::MINUS_ASSIGN)
                || check(Token::TIMES_ASSIGN)
                || check(Token::DIV_ASSIGN)
        ) {
            BinaryExp::Operation op = tokenTypeToBinaryOperation(currentToken().type);
            scanner->next();

            Exp* lhs = new Variable(line, col, id);
            Exp* rhs = parseExpression();

            ensureSemicolon("expected ';' after compound assignment statement");

            return new CompoundAssignStmt(line, col, op, lhs, rhs);
        }
        else {
            if (match(Token::OPEN_BRACKET)) {
                Exp* exp = parseExpression();
                if (!match(Token::CLOSE_BRACKET)) {
                    throw std::runtime_error("expected ']' to close subscript expression"
                                             + std::string(" in statement\ngot: ")
                                             + debugInfo(currentToken()));
                }
                Exp* lhs = new SubscriptExp(line, col, id, exp);
                if (!match(Token::ASSIGN)) {
                    if (check(Token::PLUS_ASSIGN)
                        || check(Token::MINUS_ASSIGN)
                        || check(Token::TIMES_ASSIGN)
                        || check(Token::DIV_ASSIGN)
                    ) {
                        BinaryExp::Operation op = tokenTypeToBinaryOperation(currentToken().type);
                        scanner->next();

                        Exp* rhs = parseExpression();
                        ensureSemicolon("expected ';' after compound assignment statement");

                        return new CompoundAssignStmt(line, col, op, lhs, rhs);
                    }
                    else {
                        // if it is not an assign statement, it must be an expression statement
                        scanner->restoreSnapshot(snapshot);
                    }
                }
                else {
                    Exp* rhs = parseExpression();
                    ensureSemicolon("expected ';' after compound assignment statement");

                    return new AssignStmt(line, col, lhs, rhs);
                }
            }
            else {
                scanner->restoreSnapshot(snapshot);
            }
        }
    }
    // it's equivalent to else (if no return was triggered, do this)
    Exp* exp = parseExpression();
    if (match(Token::SEMICOLON)) {
        return new ExpStmt(line, col, exp);
    }
    if (check(Token::CLOSE_CURLY)) {
        return new ExpStmt(line, col, exp, true);
    }
    throw std::runtime_error("expected valid statement\ngot: "
                             + debugInfo(currentToken()));
}

Exp* Parser::parseExpression() {
    auto [line, col] = getPos();
    Exp* exp = parseNotExp();
    if (check(Token::LOR)
        || check(Token::LAND)
    ) {
        BinaryExp::Operation op = tokenTypeToBinaryOperation(currentToken().type);
        scanner->next();

        Exp* rhs = parseExpression();
        exp = new BinaryExp(line, col, op, exp, rhs);
    }
    return exp;
}

Exp* Parser::parseNotExp() {
    auto [line, col] = getPos();
    if (match(Token::LNOT)) {
        Exp* exp = parseExpression();
        return new UnaryExp(line, col, UnaryExp::LNOT, exp);
    }
    else {
        return parseRelationalExp();
    }
}

Exp* Parser::parseRelationalExp() {
    auto [line, col] = getPos();
    Exp* exp = parseArithmeticExp();
    if (check(Token::EQ)
        || check(Token::GT)
        || check(Token::LT)
        || check(Token::GE)
        || check(Token::LE)
        || check(Token::EQ)
        || check(Token::NEQ)
    ) {
        BinaryExp::Operation op = tokenTypeToBinaryOperation(currentToken().type);
        scanner->next();

        Exp* rhs = parseArithmeticExp();
        exp = new BinaryExp(line, col, op, exp, rhs);
    }
    return exp;
}

Exp* Parser::parseArithmeticExp() {
    auto [line, col] = getPos();
    Exp* exp = parseTermExp();
    if (check(Token::PLUS)
        || check(Token::MINUS)
    ) {
        BinaryExp::Operation op = tokenTypeToBinaryOperation(currentToken().type);
        scanner->next();

        Exp* rhs = parseArithmeticExp();
        exp = new BinaryExp(line, col, op, exp, rhs);
    }
    return exp;
}

Exp* Parser::parseTermExp() {
    auto [line, col] = getPos();
    Exp* exp = parseReferenceFactorExp();
    if (check(Token::TIMES)
        || check(Token::DIV)
    ) {
        BinaryExp::Operation op = tokenTypeToBinaryOperation(currentToken().type);
        scanner->next();

        Exp* rhs = parseTermExp();
        exp = new BinaryExp(line, col, op, exp, rhs);
    }
    return exp;
}

Exp* Parser::parseReferenceFactorExp() {
    auto [line, col] = getPos();
    if (match(Token::REFERENCE)) {
        // int count {1};
        // while (match(Token::REFERENCE)) {
        //     ++count;
        // }
        Exp* exp = parseFactorExp();
        return new ReferenceExp(line, col, exp);
    }
    return parseFactorExp();
}

Exp* Parser::parseFactorExp() {
    auto [line, col] = getPos();
    if (match(Token::OPEN_PARENTHESIS)) {
        if (match(Token::CLOSE_PARENTHESIS)) {
            return new Literal(line, col, Value(Value::UNIT, "()"));
        }
        Exp* exp = parseExpression();
        if (!match(Token::CLOSE_PARENTHESIS)) {
            throw std::runtime_error("expected closing parenthesis after expression\ngot: "
                                     + debugInfo(currentToken()));
        }
        return exp;
    }
    else if (check(Token::BOOLEAN)) {
        int value = currentToken().content == "true";
        match(Token::BOOLEAN);
        return new Literal(line, col, Value(Value::BOOL, value));
    }
    else if (check(Token::CHAR)) {
        auto value = std::string(1,currentToken().content[0]);
        match(Token::CHAR);
        return new Literal(line, col, Value(Value::CHAR, value));
    }
    else if (check(Token::NUMBER)) {
        int value = stoi(currentToken().content);
        match(Token::NUMBER);
        return new Literal(line, col, Value(Value::I32, value));
    }
    else if (check(Token::STRING)) {
        std::string value = currentToken().content;
        match(Token::STRING);
        return new Literal(line, col, Value(Value::STR, value, true));
    }
    else if (check(Token::ID)) {
        std::string id = currentToken().content;
        match(Token::ID);
        if (match(Token::OPEN_PARENTHESIS)) {
            std::list<Exp*> args;
            if (match(Token::CLOSE_PARENTHESIS)) {
                return new FunCall(line, col, id, args);
            }
            args.push_back(parseExpression());
            while (match(Token::COMMA)) {
                args.push_back(parseExpression());
            }

            if (!match(Token::CLOSE_PARENTHESIS)) {
                throw std::runtime_error("expected closing parenthesis in function call\ngot: "
                                         + debugInfo(currentToken()));
            }

            return new FunCall(line, col, id, args);
        }
        else if (match(Token::OPEN_BRACKET)) {
            Exp *start {}, *end {};
            bool inclusive {};
            if (!check(Token::RANGE_IN) && !check(Token::RANGE_EX)) {
                start = parseExpression();
            }
            if (check(Token::RANGE_IN) || check(Token::RANGE_EX)) {
                if (match(Token::RANGE_IN)) {
                    inclusive = true;
                }
                else {
                    match(Token::RANGE_EX);
                } 
                if (!check(Token::CLOSE_BRACKET)) {
                    end = parseExpression();
                }
                if (!match(Token::CLOSE_BRACKET)) {
                    throw std::runtime_error("expected ']' to close slice expression"
                                             + debugInfo(currentToken()));
                }
                return new SliceExp(line, col, id, start, end, inclusive);
            }
            else {
                if (!match(Token::CLOSE_BRACKET)) {
                    throw std::runtime_error("expected ']' to close subscript expression"
                                             + debugInfo(currentToken()));
                }
                return new SubscriptExp(line, col, id, start);
            }
        }
        else {
            return new Variable(line, col, id);
        }

    }
    else if (match(Token::IF)) {
        Exp* cond = parseExpression();
        Block* block = parseBlock();

        IfExp* exp = new IfExp(line, col, cond, block);

        while (check(Token::ELSE) && peek() == Token::IF) {
            match(Token::ELSE);
            match(Token::IF);
            cond = parseExpression();
            block = parseBlock();
            exp->addElseIfBranch(cond, block);
        }

        if (match(Token::ELSE)) {
            cond = nullptr;
            block = parseBlock();
            exp->setElseBranch(cond, block);
        }

        return exp;
    }
    else if (match(Token::LOOP)) {
        return new LoopExp(line, col, parseBlock());
    }
    else {
        throw std::runtime_error("expected valid factor expression\ngot: "
                                 + debugInfo(currentToken()));
    }
}

