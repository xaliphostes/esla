#pragma once

#include "Token.h"
#include <string>
#include <vector>

namespace esla {

/**
 * @class Lexer
 * @brief Converts source code into tokens.
 *
 * The Lexer scans through the source code and generates a sequence of tokens
 * that represent the lexical elements of the language.
 */
class Lexer {
  private:
    std::string source;
    size_t start = 0;
    size_t current = 0;
    std::vector<Token> tokens;

    /**
     * @brief Checks if the current position is at the end of the source code.
     * @return true if at the end, false otherwise.
     */
    bool isAtEnd() const;

    /**
     * @brief Advances the current position and returns the character.
     * @return The character at the current position before advancing.
     */
    char advance();

    /**
     * @brief Peeks at the current character without advancing.
     * @return The character at the current position.
     */
    char peek() const;

    /**
     * @brief Peeks at the next character without advancing.
     * @return The character after the current position.
     */
    char peekNext() const;

    /**
     * @brief Checks if the current character matches the expected character.
     * @param expected The expected character.
     * @return true if the current character matches, false otherwise.
     */
    bool match(char expected);

    /**
     * @brief Adds a token to the tokens list.
     * @param type The token type.
     * @param literal The token literal value (optional).
     */
    void addToken(TokenType type,
                  const std::shared_ptr<Value> &literal = nullptr);

    /**
     * @brief Scans the next token.
     */
    void scanToken();

    /**
     * @brief Scans a number (integer or double).
     */
    void number();

    /**
     * @brief Scans a string.
     */
    void string();

    /**
     * @brief Scans an identifier or keyword.
     */
    void identifier();

  public:
    /**
     * @brief Constructs a Lexer with the given source code.
     * @param source The source code to scan.
     */
    Lexer(const std::string &source);

    /**
     * @brief Scans the source code and returns the tokens.
     * @return The tokens.
     */
    std::vector<Token> scanTokens();
};

} // namespace esla
