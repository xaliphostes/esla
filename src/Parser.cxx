#include <scripter/Parser.h>
#include <scripter/AST.h>
#include <iostream>
#include <stdexcept>

Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens), current(0) {}

std::shared_ptr<Node> Parser::parse() {
    try {
        return statement();
    } catch (const std::exception &e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        return nullptr;
    }
}

std::vector<std::shared_ptr<Node>> Parser::parseAll() {
    std::vector<std::shared_ptr<Node>> statements;
    try {
        while (!isAtEnd()) {
            statements.push_back(statement());
        }
        return statements;
    } catch (const std::exception &e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        return {};
    }
}

Token Parser::peek() const { return tokens[current]; }

Token Parser::previous() const { return tokens[current - 1]; }

bool Parser::isAtEnd() const { return peek().type == TokenType::EOF_TOKEN; }

Token Parser::advance() {
    if (!isAtEnd())
        current++;
    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd())
        return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::match(const std::initializer_list<TokenType> &types) {
    for (const auto &type : types) {
        if (match(type))
            return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string &message) {
    if (check(type))
        return advance();
    throw std::runtime_error(message);
}

std::shared_ptr<Node> Parser::statement() {
    if (match(TokenType::FUNCTION)) {
        return functionDeclaration();
    }

    if (match(TokenType::RETURN)) {
        return returnStatement();
    }

    if (match(TokenType::LBRACE)) {
        return blockStatement();
    }

    return expressionStatement();
}

std::shared_ptr<Node> Parser::functionDeclaration() {
    std::string name;
    if (match(TokenType::IDENTIFIER)) {
        name = previous().lexeme;
    } else {
        throw std::runtime_error("Expected function name.");
    }

    consume(TokenType::LPAREN, "Expected '(' after function name.");

    std::vector<std::string> parameters;
    if (!check(TokenType::RPAREN)) {
        do {
            if (parameters.size() >= 255) {
                throw std::runtime_error(
                    "Cannot have more than 255 parameters.");
            }

            consume(TokenType::IDENTIFIER, "Expected parameter name.");
            parameters.push_back(previous().lexeme);
        } while (match(TokenType::COMMA));
    }

    consume(TokenType::RPAREN, "Expected ')' after parameters.");
    consume(TokenType::LBRACE, "Expected '{' before function body.");

    auto body = blockStatement();

    return std::make_shared<FunctionNode>(name, std::move(parameters),
                                          std::move(body));
}

std::shared_ptr<Node> Parser::returnStatement() {
    std::shared_ptr<Node> value = nullptr;

    if (!check(TokenType::SEMICOLON) && !check(TokenType::RBRACE)) {
        value = expression();
    }

    // Optional semicolon
    match(TokenType::SEMICOLON);

    return std::make_shared<ReturnNode>(std::move(value));
}

std::shared_ptr<Node> Parser::blockStatement() {
    std::vector<std::shared_ptr<Node>> statements;

    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        statements.push_back(statement());
    }

    consume(TokenType::RBRACE, "Expected '}' after block.");

    return std::make_shared<Block>(std::move(statements));
}

std::shared_ptr<Node> Parser::expressionStatement() {
    auto expr = expression();
    // Optional semicolon
    match(TokenType::SEMICOLON);
    return std::make_shared<ExpressionStmt>(std::move(expr));
}

std::shared_ptr<Node> Parser::expression() { return assignment(); }

std::shared_ptr<Node> Parser::assignment() {
    auto expr = addition();

    if (match(TokenType::EQUALS)) {
        auto value = expression();

        if (auto *varExpr = dynamic_cast<VariableNode *>(expr.get())) {
            std::string name = varExpr->getName();
            return std::make_shared<AssignNode>(name, std::move(value));
        }

        throw std::runtime_error("Invalid assignment target.");
    }

    return expr;
}

std::shared_ptr<Node> Parser::addition() {
    auto expr = multiplication();

    while (match({TokenType::PLUS, TokenType::MINUS})) {
        Token op = previous();
        auto right = multiplication();
        expr =
            std::make_shared<BinaryNode>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::shared_ptr<Node> Parser::multiplication() {
    auto expr = call();

    while (match({TokenType::MULTIPLY, TokenType::DIVIDE})) {
        Token op = previous();
        auto right = call();
        expr =
            std::make_shared<BinaryNode>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::shared_ptr<Node> Parser::call() {
    auto expr = primary();

    while (true) {
        if (match(TokenType::LPAREN)) {
            expr = finishCall(std::move(expr));
        } else {
            break;
        }
    }

    return expr;
}

std::shared_ptr<Node> Parser::finishCall(std::shared_ptr<Node> callee) {
    std::vector<std::shared_ptr<Node>> arguments;

    if (!check(TokenType::RPAREN)) {
        do {
            if (arguments.size() >= 255) {
                throw std::runtime_error(
                    "Cannot have more than 255 arguments.");
            }
            arguments.push_back(expression());
        } while (match(TokenType::COMMA));
    }

    consume(TokenType::RPAREN, "Expected ')' after arguments.");

    return std::make_shared<CallNode>(std::move(callee), std::move(arguments));
}

std::shared_ptr<Node> Parser::primary() {
    if (match(TokenType::NUMBER) || match(TokenType::STRING)) {
        return std::make_shared<LiteralNode>(previous().literal);
    }

    if (match(TokenType::IDENTIFIER)) {
        return std::make_shared<VariableNode>(previous().lexeme);
    }

    if (match(TokenType::LPAREN)) {
        auto expr = expression();
        consume(TokenType::RPAREN, "Expected ')' after expression.");
        return expr;
    }

    throw std::runtime_error("Expected expression.");
}