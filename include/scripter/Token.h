#pragma once

#include "Value.h"
#include <string>
#include <memory>

/**
 * @enum TokenType
 * @brief Enumeration of all possible token types in the language.
 * 
 * This enumeration defines all the token types that can be recognized by the lexer
 * and processed by the parser.
 */
enum class TokenType {
    // Literals
    NUMBER,     // Any numeric value (int, float, bool)
    STRING,     // String literals enclosed in single quotes: 'string'
    IDENTIFIER, // Variable and function names
    
    // Operators
    EQUALS,     // =
    PLUS,       // +
    MINUS,      // -
    MULTIPLY,   // *
    DIVIDE,     // /
    
    // Punctuation
    LPAREN,     // (
    RPAREN,     // )
    LBRACE,     // {
    RBRACE,     // }
    COMMA,      // ,
    SEMICOLON,  // ;
    
    // Keywords
    FUNCTION,   // function
    RETURN,     // return
    
    // Special tokens
    EOF_TOKEN   // End of file
};

/**
 * @struct Token
 * @brief Represents a token in the language.
 * 
 * A token is the smallest unit of the language that has meaning. This struct
 * stores the type, lexeme (the actual string from the source code), and any
 * literal value associated with the token.
 */
struct Token {
    TokenType type;
    std::string lexeme;
    std::shared_ptr<Value> literal;

    /**
     * @brief Constructs a token with the given type, lexeme, and optional literal value.
     * @param type The token type.
     * @param lexeme The string from the source code that this token represents.
     * @param literal The literal value associated with this token (if any).
     */
    Token(TokenType type, const std::string& lexeme, const std::shared_ptr<Value>& literal = nullptr);
    
    /**
     * @brief Get a string representation of this token.
     * @return A string representation of the token.
     */
    std::string toString() const;
};
