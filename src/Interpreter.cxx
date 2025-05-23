#include <esla/AST.h>
#include <esla/Environment.h>
#include <esla/Interpreter.h>
#include <esla/Lexer.h>
#include <esla/Parser.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace esla {

// Interpreter implementation
Interpreter::Interpreter() {
    globals = std::make_shared<Environment>();
    environment = globals;

    // Define built-in functions
    // This is where you would add native functions like print, input, etc.

    // Example: Define a 'print' function that prints a value
    auto printFunc = std::make_shared<FunctionObject>(
        std::vector<std::string>{"value"}, // Parameter names
        nullptr, // Body is nullptr for native functions
        globals  // Closure environment
    );

    // Native function implementation could be handled in the CallNode evaluate
    // method by checking if the body is nullptr and then calling a native
    // handler
}

std::shared_ptr<Value> Interpreter::evaluate(std::shared_ptr<Node> node) {
    if (!node)
        return std::make_shared<Value>();

    try {
        return node->evaluate(*this);
    } catch (const ReturnValue &returnValue) {
        // Re-throw returns from top-level code
        throw returnValue;
    }
}

std::shared_ptr<Value>
Interpreter::executeBlock(const std::vector<std::shared_ptr<Node>> &statements,
                          std::shared_ptr<Environment> environment) {
    std::shared_ptr<Environment> previous = this->environment;

    try {
        this->environment = environment;

        std::shared_ptr<Value> lastValue = std::make_shared<Value>();
        for (const auto &statement : statements) {
            lastValue = evaluate(statement);
        }

        this->environment = previous;
        return lastValue;
    } catch (const ReturnValue &returnValue) {
        this->environment = previous;
        throw returnValue;
    } catch (const std::exception &e) {
        this->environment = previous;
        throw e;
    }
}

std::shared_ptr<Value> Interpreter::lookUpVariable(const std::string &name) {
    try {
        return environment->get(name);
    } catch (const std::exception &e) {
        throw std::runtime_error("Undefined variable '" + name + "'.");
    }
}

void Interpreter::assign(const std::string &name,
                         std::shared_ptr<Value> value) {
    try {
        environment->assign(name, std::move(value));
    } catch (const std::exception &) {
        // If variable not found in current scope, define in globals
        globals->define(name, std::move(value));
    }
}

void Interpreter::define(const std::string &name,
                         std::shared_ptr<Value> value) {
    environment->define(name, std::move(value));
}

std::shared_ptr<Environment> Interpreter::getEnvironment() {
    return environment;
}

std::shared_ptr<Environment> Interpreter::getGlobals() { return globals; }

void Interpreter::printValue(const std::shared_ptr<Value> &value) {
    if (!value) {
        std::cout << "null";
        return;
    }

    std::cout << value->toString();
}

void Interpreter::printVariables() {
    for (const auto &[name, value] : globals->getValues()) {
        std::cout << name << " = ";
        printValue(value);
        std::cout << std::endl;
    }
}

// Helper functions for operations
std::shared_ptr<Value> Interpreter::add(const std::shared_ptr<Value> &a,
                                        const std::shared_ptr<Value> &b) {
    if (a->isString() && b->isString()) {
        return std::make_shared<Value>(a->getString() + b->getString());
    }

    if (a->isNumber() && b->isNumber()) {
        if (a->isInt() && b->isInt()) {
            return std::make_shared<Value>(a->getInt() + b->getInt());
        }
        return std::make_shared<Value>(a->asDouble() + b->asDouble());
    }

    throw std::runtime_error("Operands must be numbers or strings.");
}

std::shared_ptr<Value> Interpreter::subtract(const std::shared_ptr<Value> &a,
                                             const std::shared_ptr<Value> &b) {
    if (a->isNumber() && b->isNumber()) {
        if (a->isInt() && b->isInt()) {
            return std::make_shared<Value>(a->getInt() - b->getInt());
        }
        return std::make_shared<Value>(a->asDouble() - b->asDouble());
    }

    throw std::runtime_error("Operands must be numbers.");
}

std::shared_ptr<Value> Interpreter::multiply(const std::shared_ptr<Value> &a,
                                             const std::shared_ptr<Value> &b) {
    if (a->isNumber() && b->isNumber()) {
        if (a->isInt() && b->isInt()) {
            return std::make_shared<Value>(a->getInt() * b->getInt());
        }
        return std::make_shared<Value>(a->asDouble() * b->asDouble());
    }

    throw std::runtime_error("Operands must be numbers.");
}

std::shared_ptr<Value> Interpreter::divide(const std::shared_ptr<Value> &a,
                                           const std::shared_ptr<Value> &b) {
    if (a->isNumber() && b->isNumber()) {
        // Check for division by zero
        if ((b->isInt() && b->getInt() == 0) ||
            (b->isDouble() && b->getDouble() == 0.0)) {
            throw std::runtime_error("Division by zero.");
        }

        if (a->isInt() && b->isInt() && a->getInt() % b->getInt() == 0) {
            return std::make_shared<Value>(a->getInt() / b->getInt());
        }

        return std::make_shared<Value>(a->asDouble() / b->asDouble());
    }

    throw std::runtime_error("Operands must be numbers.");
}

std::shared_ptr<Value> Interpreter::evaluate(const std::string &source) {
    try {
        // Tokenize the source code
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.scanTokens();

        // Parse into AST
        Parser parser(tokens);
        auto statement = parser.parse();

        if (!statement) {
            throw std::runtime_error("Failed to parse expression");
        }

        // Evaluate the AST
        return evaluate(statement);
    } catch (const std::exception &e) {
        throw std::runtime_error("Evaluation error: " + std::string(e.what()));
    }
}

std::shared_ptr<Value> Interpreter::execute(const std::string &source) {
    try {
        // Tokenize the source code
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.scanTokens();

        // Parse all statements
        Parser parser(tokens);
        auto statements = parser.parseAll();

        if (statements.empty()) {
            return std::make_shared<Value>(); // Return null for empty script
        }

        // Execute all statements
        std::shared_ptr<Value> lastResult = std::make_shared<Value>();
        for (const auto &statement : statements) {
            lastResult = evaluate(statement);
        }

        return lastResult;
    } catch (const std::exception &e) {
        throw std::runtime_error("Execution error: " + std::string(e.what()));
    }
}

std::string Interpreter::evaluateToString(const std::string &source) {
    auto result = evaluate(source);
    return result ? result->toString() : "null";
}

void Interpreter::run(const std::string &source) {
    execute(source); // Execute and discard the result
}

std::shared_ptr<Value> Interpreter::executeFile(const std::string &filename) {
    // Read the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    // Execute the file contents
    return execute(source);
}

} // namespace esla
