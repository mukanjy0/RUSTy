#include "Parser.h"

Parser::Parser(char* filename) {
    scanner = new Scanner(filename);
    match(Token::BEG);
}
Parser::~Parser() {
    delete scanner;
}

std::string Parser::debugInfo(Token token) {
    return token.content
             + "\nat line: " + std::to_string(token.line)
             + " | column: " + std::to_string(token.col);
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
        default: 
            throw std::runtime_error("expected binary operation token\ngot: " 
                                     + debugInfo(currentToken()));
    }
}

Token::Type Parser::peek() {
    return scanner->peek();
}
bool Parser::check(Token::Type type) {
    return scanner->check(type);
}
bool Parser::match(Token::Type type) {
    return scanner->match(type);
}
Token Parser::currentToken() {
    return scanner->getToken();
}

Program* Parser::parse() {
    std::map<std::string,Fun*> funs;
    auto [id, fun] = parseFunction();
    funs[id] = fun;
    while (!scanner->eof()) {
        auto [id, fun] = parseFunction();
        funs[id] = fun;
    }
    return new Program(funs);
}

Block* Parser::parseBlock() {
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

    return new Block(stmts);
}

Param Parser::parseParameter() {
    Param param;
    if (!check(Token::TYPE)) {
        throw std::runtime_error("expected type in parameter list\ngot: " 
                                 + debugInfo(currentToken()));
    }
    param.type = Var::stringToType(currentToken().content);
    match(Token::TYPE);

    if (!check(Token::ID)) {
        throw std::runtime_error("expected id in parameter list\ngot: " 
                                 + debugInfo(currentToken()));
    }
    param.id = currentToken().content;
    match(Token::ID);

    return param;
}

std::pair<std::string, Fun*> Parser::parseFunction() {
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

    Block* block = parseBlock();

    return {id, new Fun(params, block)};
}

Stmt* Parser::parseStatement() {
    if (match(Token::LET)) {
        Var var {};
        if (match(Token::MUT)) {
            var.mut = true;
        }
        std::string id = currentToken().content;
        if (!match(Token::ID)) {
            throw std::runtime_error("expected id in variable declaration\ngot: " 
                                     + debugInfo(currentToken()));
        }
        if (match(Token::COLON)) {
            if (!check(Token::TYPE)) {
                throw std::runtime_error("expected type after ':' in variable declaration\ngot: " 
                                         + debugInfo(currentToken()));
            }
            var.type = Var::stringToType(currentToken().content);
            match(Token::TYPE);
        }
        if (match(Token::SEMICOLON)) {
            return new DecStmt(id, var);
        }
        if (!match(Token::ASSIGN)) {
            throw std::runtime_error("expected '=' in variable declaration\ngot: " 
                                     + debugInfo(currentToken()));
        }
        Exp* rhs = parseExpression();
        if (!match(Token::SEMICOLON)) {
            throw std::runtime_error("expected ';' after declaration statement\ngot: " 
                                     + debugInfo(currentToken()));
        }

        return new DecStmt(id, var, rhs);

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

        return new ForStmt(id, start, end, block, inclusive);
    }
    else if (match(Token::WHILE)) {
        Exp* cond = parseExpression();
        Block* block = parseBlock();

        return new WhileStmt(cond, block);
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

        if (!match(Token::SEMICOLON)) {
            throw std::runtime_error("expected ';' after print statement\ngot: " 
                                     + debugInfo(currentToken()));
        }

        return new PrintStmt(literal, args);
    }
    else if (match(Token::BREAK)) {
        // if break is last statement in block, it can be missing ';'
        if (match(Token::SEMICOLON) || check(Token::CLOSE_CURLY)) {
            return new BreakStmt();
        }
        Exp* exp = parseExpression();
        // if break is last statement in block, it can be missing ';'
        if (match(Token::SEMICOLON) || check(Token::CLOSE_CURLY)) {
            throw std::runtime_error("expected ';' after break expression\ngot: " 
                                     + debugInfo(currentToken()));
        }

        return new BreakStmt(exp);
    }
    else if (match(Token::RETURN)) {
        // if return is last statement in block, it can be missing ';'
        if (match(Token::SEMICOLON) || check(Token::CLOSE_CURLY)) {
            return new ReturnStmt();
        }
        Exp* exp = parseExpression();
        // if return is last statement in block, it can be missing ';'
        if (match(Token::SEMICOLON) || check(Token::CLOSE_CURLY)) {
            throw std::runtime_error("expected ';' after break expression\ngot: " 
                                     + debugInfo(currentToken()));
        }

        return new ReturnStmt(exp);
    }
    else if (check(Token::ID) && peek() == Token::ASSIGN) {
        std::string id = currentToken().content;
        match(Token::ID);
        match(Token::ASSIGN);
        Exp* rhs = parseExpression();

        if (!match(Token::SEMICOLON)) {
            throw std::runtime_error("expected ';' after assignment statement\ngot: " 
                                     + debugInfo(currentToken()));
        }

        return new AssignStmt(id, rhs);
    }
    else {
        Exp* exp = parseExpression();
        if (match(Token::SEMICOLON)) {
            return new ExpStmt(exp);
        }
        if (check(Token::CLOSE_CURLY)) {
            return new ExpStmt(exp, true);
        }
        throw std::runtime_error("expected valid statement\ngot: " 
                                 + debugInfo(currentToken()));
    }
}

Exp* Parser::parseExpression() {
    Exp* exp = parseNotExp();
    if (check(Token::LOR) 
        || check(Token::LAND)
    ) {
        Exp* rhs = parseExpression();
        exp = new BinaryExp(tokenTypeToBinaryOperation(currentToken().type), exp, rhs);
        scanner->next();
    }
    return exp;
}

Exp* Parser::parseNotExp() {
    if (match(Token::LNOT)) {
        Exp* exp = parseExpression();
        return new UnaryExp(UnaryExp::LNOT, exp);
    }
    else {
        return parseRelationalExp();
    }
}

Exp* Parser::parseRelationalExp() {
    Exp* exp = parseArithmeticExp();
    if (check(Token::EQ) 
        || check(Token::GT)
        || check(Token::LT)
        || check(Token::GE)
        || check(Token::LE)
        || check(Token::EQ)
        || check(Token::NEQ)
    ) {
        Exp* rhs = parseArithmeticExp();
        exp = new BinaryExp(tokenTypeToBinaryOperation(currentToken().type), exp, rhs);
        scanner->next();
    }
    return exp;
}

Exp* Parser::parseArithmeticExp() {
    Exp* exp = parseTermExp();
    if (check(Token::PLUS) 
        || check(Token::MINUS)
    ) {
        Exp* rhs = parseArithmeticExp();
        exp = new BinaryExp(tokenTypeToBinaryOperation(currentToken().type), exp, rhs);
        scanner->next();
    }
    return exp;
}

Exp* Parser::parseTermExp() {
    Exp* exp = parseFactorExp();
    if (check(Token::TIMES)
        || check(Token::DIV)
    ) {
        Exp* rhs = parseTermExp();
        exp = new BinaryExp(tokenTypeToBinaryOperation(currentToken().type), exp, rhs);
        scanner->next();
    }
    return exp;
}

Exp* Parser::parseFactorExp() {
    if (match(Token::OPEN_PARENTHESIS)) {
        Exp* exp = parseExpression();
        if (!match(Token::CLOSE_PARENTHESIS)) {
            throw std::runtime_error("expected closing parenthesis after expression\ngot: " 
                                     + debugInfo(currentToken()));
        }
        return exp;
    }
    else if (check(Token::NUMBER)) {
        int value = stoi(currentToken().content);
        match(Token::NUMBER);
        return new Number(value);
    }
    else if (check(Token::ID)) {
        std::string id = currentToken().content;
        match(Token::ID);
        if (!match(Token::OPEN_PARENTHESIS)) {
            return new Variable(id);
        }

        std::list<Exp*> args;
        if (match(Token::CLOSE_PARENTHESIS)) {
            return new FunCall(id, args);
        }
        args.push_back(parseExpression());
        while (match(Token::COMMA)) {
            args.push_back(parseExpression());
        }

        if (!match(Token::CLOSE_PARENTHESIS)) {
            throw std::runtime_error("expected closing parenthesis in function call\ngot: " 
                                     + debugInfo(currentToken()));
        }

        return new FunCall(id, args);
    }
    else if (match(Token::IF)) {
        IfExp* exp;
        Exp* cond = parseExpression();
        Block* block = parseBlock();
        exp->setIfBranch(IfBranch(cond, block));

        std::list<IfBranch> elseIfBranches;
        while (check(Token::ELSE) && peek() == Token::IF) {
            match(Token::ELSE);
            match(Token::IF);
            cond = parseExpression();
            block = parseBlock();
            elseIfBranches.emplace_back(cond, block);
        }

        if (match(Token::ELSE)) {
            cond = parseExpression();
            block = parseBlock();
            exp->setElseBranch(new IfBranch(cond, block));
        }

        return exp;
    }
    else if (match(Token::LOOP)) {
        return new LoopExp(parseBlock());
    }
    else {
        throw std::runtime_error("expected valid factor expression\ngot: "
                                 + debugInfo(currentToken()));
    }
}

