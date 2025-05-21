#include <esla/Token.h>
#include <sstream>

namespace esla {

Token::Token(TokenType type, const std::string &lexeme,
             const std::shared_ptr<Value> &literal)
    : type(type), lexeme(lexeme), literal(literal) {}

std::string Token::toString() const {
    std::stringstream ss;

    // Convert token type to string
    std::string typeStr;
    switch (type) {
    case TokenType::NUMBER:
        typeStr = "NUMBER";
        break;
    case TokenType::STRING:
        typeStr = "STRING";
        break;
    case TokenType::IDENTIFIER:
        typeStr = "IDENTIFIER";
        break;

    // Operators
    case TokenType::EQUALS:
        typeStr = "EQUALS";
        break;
    case TokenType::PLUS:
        typeStr = "PLUS";
        break;
    case TokenType::MINUS:
        typeStr = "MINUS";
        break;
    case TokenType::MULTIPLY:
        typeStr = "MULTIPLY";
        break;
    case TokenType::DIVIDE:
        typeStr = "DIVIDE";
        break;

    // Comparison operators
    case TokenType::LESS:
        typeStr = "LESS";
        break;
    case TokenType::LESS_EQUAL:
        typeStr = "LESS_EQUAL";
        break;
    case TokenType::GREATER:
        typeStr = "GREATER";
        break;
    case TokenType::GREATER_EQUAL:
        typeStr = "GREATER_EQUAL";
        break;
    case TokenType::EQUAL_EQUAL:
        typeStr = "EQUAL_EQUAL";
        break;
    case TokenType::NOT_EQUAL:
        typeStr = "NOT_EQUAL";
        break;

    // Punctuation
    case TokenType::LPAREN:
        typeStr = "LPAREN";
        break;
    case TokenType::RPAREN:
        typeStr = "RPAREN";
        break;
    case TokenType::LBRACE:
        typeStr = "LBRACE";
        break;
    case TokenType::RBRACE:
        typeStr = "RBRACE";
        break;
    case TokenType::COMMA:
        typeStr = "COMMA";
        break;
    case TokenType::SEMICOLON:
        typeStr = "SEMICOLON";
        break;

    // Keywords
    case TokenType::FUNCTION:
        typeStr = "FUNCTION";
        break;
    case TokenType::RETURN:
        typeStr = "RETURN";
        break;
    case TokenType::IF:
        typeStr = "IF";
        break;
    case TokenType::ELSE:
        typeStr = "ELSE";
        break;
    case TokenType::WHILE:
        typeStr = "WHILE";
        break;
    case TokenType::FOR:
        typeStr = "FOR";
        break;

    case TokenType::EOF_TOKEN:
        typeStr = "EOF";
        break;
    default:
        typeStr = "UNKNOWN";
        break;
    }

    ss << "Token(" << typeStr << ", '" << lexeme << "'";

    // Add literal value if present
    if (literal) {
        ss << ", " << literal->toString();
    }

    ss << ")";
    return ss.str();
}

// Utility functions to help with token operations

/**
 * @brief Determine if a token is a keyword.
 * @param type The token type to check.
 * @return true if the token is a keyword, false otherwise.
 */
bool isKeyword(TokenType type) {
    return type == TokenType::FUNCTION || type == TokenType::RETURN ||
           type == TokenType::IF || type == TokenType::ELSE ||
           type == TokenType::WHILE || type == TokenType::FOR;
}

/**
 * @brief Determine if a token is an operator.
 * @param type The token type to check.
 * @return true if the token is an operator, false otherwise.
 */
bool isOperator(TokenType type) {
    return type == TokenType::EQUALS || type == TokenType::PLUS ||
           type == TokenType::MINUS || type == TokenType::MULTIPLY ||
           type == TokenType::DIVIDE || type == TokenType::LESS ||
           type == TokenType::LESS_EQUAL || type == TokenType::GREATER ||
           type == TokenType::GREATER_EQUAL || type == TokenType::EQUAL_EQUAL ||
           type == TokenType::NOT_EQUAL;
}

/**
 * @brief Get the precedence of an operator token.
 * @param type The token type to check.
 * @return The precedence level (higher means higher precedence), or 0 if not an
 * operator.
 */
int getOperatorPrecedence(TokenType type) {
    switch (type) {
    // Comparison operators
    case TokenType::EQUAL_EQUAL:
    case TokenType::NOT_EQUAL:
        return 3;
    case TokenType::LESS:
    case TokenType::LESS_EQUAL:
    case TokenType::GREATER:
    case TokenType::GREATER_EQUAL:
        return 4;

    // Arithmetic operators
    case TokenType::PLUS:
    case TokenType::MINUS:
        return 5;
    case TokenType::MULTIPLY:
    case TokenType::DIVIDE:
        return 6;

    // Assignment
    case TokenType::EQUALS:
        return 1;

    default:
        return 0;
    }
}

/**
 * @brief Check if an operator is left-associative.
 * @param type The token type to check.
 * @return true if the operator is left-associative, false otherwise.
 */
bool isLeftAssociative(TokenType type) {
    // All our operators except assignment are left-associative
    return type != TokenType::EQUALS;
}

/**
 * @brief Convert a string to a token type if it's a keyword.
 * @param identifier The identifier to check.
 * @return The corresponding token type if the identifier is a keyword, or
 * TokenType::IDENTIFIER otherwise.
 */
TokenType getKeywordType(const std::string &identifier) {
    if (identifier == "function")
        return TokenType::FUNCTION;
    if (identifier == "return")
        return TokenType::RETURN;
    if (identifier == "if")
        return TokenType::IF;
    if (identifier == "else")
        return TokenType::ELSE;
    if (identifier == "while")
        return TokenType::WHILE;
    if (identifier == "for")
        return TokenType::FOR;

    return TokenType::IDENTIFIER;
}

} // namespace esla