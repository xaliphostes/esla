#pragma once

#include "Token.h"
#include "Node.h"
#include <vector>
#include <memory>
#include <initializer_list>

namespace esla {

// Forward declarations
class Node;
class FunctionNode;
class ReturnNode;
class Block;
class ExpressionStmt;
class BinaryNode;
class AssignNode;
class VariableNode;
class LiteralNode;
class CallNode;

/**
 * @class Parser
 * @brief Parses tokens into an abstract syntax tree.
 * 
 * The Parser takes a list of tokens produced by the Lexer and transforms them
 * into an abstract syntax tree (AST). This AST represents the program structure
 * and can be executed by the Interpreter.
 */
class Parser {
public:
    /**
     * @brief Constructs a Parser with the given tokens.
     * @param tokens The tokens to parse.
     */
    Parser(const std::vector<Token>& tokens);
    
    /**
     * @brief Parses a single statement.
     * @return A pointer to the root of the AST.
     */
    std::shared_ptr<Node> parse();
    
    /**
     * @brief Parses a sequence of statements.
     * @return A vector of statement nodes.
     */
    std::vector<std::shared_ptr<Node>> parseAll();
    
private:
    std::vector<Token> tokens;
    size_t current;
    
    // Helper methods for the parser
    Token peek() const;
    Token previous() const;
    bool isAtEnd() const;
    Token advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    bool match(const std::initializer_list<TokenType>& types);
    Token consume(TokenType type, const std::string& message);
    
    // Grammar production rules
    std::shared_ptr<Node> statement();
    std::shared_ptr<Node> functionDeclaration();
    std::shared_ptr<Node> returnStatement();
    std::shared_ptr<Node> blockStatement();
    std::shared_ptr<Node> expressionStatement();
    std::shared_ptr<Node> expression();
    std::shared_ptr<Node> assignment();
    std::shared_ptr<Node> addition();
    std::shared_ptr<Node> multiplication();
    std::shared_ptr<Node> call();
    std::shared_ptr<Node> finishCall(std::shared_ptr<Node> callee);
    std::shared_ptr<Node> primary();
    std::shared_ptr<Node> memberAccess();
};

}
