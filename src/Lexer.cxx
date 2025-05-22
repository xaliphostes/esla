#include <cctype>
#include <esla/Lexer.h>
#include <iostream>

namespace esla {

Lexer::Lexer(const std::string &source) : source(source) {}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        // We are at the beginning of the next lexeme
        start = current;
        scanToken();
    }

    tokens.emplace_back(TokenType::EOF_TOKEN, "");
    return tokens;
}

bool Lexer::isAtEnd() const { return current >= source.length(); }

char Lexer::advance() { return source[current++]; }

char Lexer::peek() const {
    if (isAtEnd())
        return '\0';
    return source[current];
}

char Lexer::peekNext() const {
    if (current + 1 >= source.length())
        return '\0';
    return source[current + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd())
        return false;
    if (source[current] != expected)
        return false;
    current++;
    return true;
}

void Lexer::addToken(TokenType type, const std::shared_ptr<Value> &literal) {
    std::string lexeme = source.substr(start, current - start);
    tokens.emplace_back(type, lexeme, literal);
}

void Lexer::scanToken() {
    char c = advance();
    switch (c) {
    case '=':
        if (match('=')) {
            addToken(TokenType::EQUAL_EQUAL);
        } else if (match('>')) {
            addToken(TokenType::GREATER_EQUAL);
        } else {
            addToken(TokenType::EQUALS);
        }
        break;
    case '<':
        if (match('=')) {
            addToken(TokenType::LESS_EQUAL);
        } else {
            addToken(TokenType::LESS);
        }
        break;
    case '>':
        if (match('=')) {
            addToken(TokenType::GREATER_EQUAL);
        } else {
            addToken(TokenType::GREATER);
        }
        break;
    case '!':
        if (match('=')) {
            addToken(TokenType::NOT_EQUAL);
        } else {
            // Handle '!' as an error or add a NOT token type
            std::cerr << "Unexpected character: !" << std::endl;
        }
        break;
    case '+':
        addToken(TokenType::PLUS);
        break;
    case '-':
        addToken(TokenType::MINUS);
        break;
    case '*':
        addToken(TokenType::MULTIPLY);
        break;
    case '/':
        // Handle comments
        if (match('/')) {
            // A comment goes until the end of the line
            while (peek() != '\n' && !isAtEnd())
                advance();
        } else {
            addToken(TokenType::DIVIDE);
        }
        break;
    case '(':
        addToken(TokenType::LPAREN);
        break;
    case ')':
        addToken(TokenType::RPAREN);
        break;
    case '{':
        addToken(TokenType::LBRACE);
        break;
    case '}':
        addToken(TokenType::RBRACE);
        break;
    case ',':
        addToken(TokenType::COMMA);
        break;
    case ';':
        addToken(TokenType::SEMICOLON);
        break;
    case ' ':
    case '\r':
    case '\t':
    case '\n':
        // Ignore whitespace
        break;
    case '\'':
        string();
        break;
    case '.':
        addToken(TokenType::DOT);
        break;
    default:
        if (std::isdigit(c)) {
            number();
        } else if (std::isalpha(c) || c == '_') {
            identifier();
        } else {
            std::cerr << "Unexpected character: " << c << std::endl;
        }
        break;
    }
}

void Lexer::number() {
    while (std::isdigit(peek()))
        advance();

    // Look for a fractional part
    if (peek() == '.' && std::isdigit(peekNext())) {
        // Consume the "."
        advance();

        while (std::isdigit(peek()))
            advance();

        // Parse as double
        double value = std::stod(source.substr(start, current - start));
        addToken(TokenType::NUMBER, std::make_shared<Value>(value));
    } else {
        // Parse as integer
        int value = std::stoi(source.substr(start, current - start));
        addToken(TokenType::NUMBER, std::make_shared<Value>(value));
    }
}

void Lexer::string() {
    while (peek() != '\'' && !isAtEnd()) {
        advance();
    }

    if (isAtEnd()) {
        std::cerr << "Unterminated string." << std::endl;
        return;
    }

    // The closing '
    advance();

    // Trim the surrounding quotes
    std::string value = source.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, std::make_shared<Value>(value));
}

void Lexer::identifier() {
    while (std::isalnum(peek()) || peek() == '_')
        advance();

    std::string text = source.substr(start, current - start);

    // Check for keywords
    TokenType type = TokenType::IDENTIFIER;
    if (text == "true") {
        addToken(TokenType::NUMBER, std::make_shared<Value>(true));
        return;
    } else if (text == "false") {
        addToken(TokenType::NUMBER, std::make_shared<Value>(false));
        return;
    } else if (text == "function") {
        type = TokenType::FUNCTION;
    } else if (text == "return") {
        type = TokenType::RETURN;
    } else if (text == "if") {
        type = TokenType::IF;
    } else if (text == "else") {
        type = TokenType::ELSE;
    } else if (text == "while") {
        type = TokenType::WHILE;
    } else if (text == "for") {
        type = TokenType::FOR;
    }

    addToken(type);
}

} // namespace esla