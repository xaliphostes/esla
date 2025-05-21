#include <esla/AST.h>
#include <esla/Environment.h>
#include <esla/Interpreter.h>
#include <sstream>

namespace esla {

// ExpressionStmt implementation
ExpressionStmt::ExpressionStmt(std::shared_ptr<Node> expression)
    : expression(std::move(expression)) {}

std::shared_ptr<Value> ExpressionStmt::evaluate(Interpreter &interpreter) {
    return expression->evaluate(interpreter);
}

std::string ExpressionStmt::toString() const {
    return expression->toString() + ";";
}

// Block implementation
Block::Block(std::vector<std::shared_ptr<Node>> statements)
    : statements(std::move(statements)) {}

std::shared_ptr<Value> Block::evaluate(Interpreter &interpreter) {
    return interpreter.executeBlock(
        statements,
        std::make_shared<Environment>(interpreter.getEnvironment()));
}

std::string Block::toString() const {
    std::stringstream ss;
    ss << "{\n";
    for (const auto &stmt : statements) {
        ss << "  " << stmt->toString() << "\n";
    }
    ss << "}";
    return ss.str();
}

const std::vector<std::shared_ptr<Node>> &Block::getStatements() const {
    return statements;
}

// LiteralNode implementation
LiteralNode::LiteralNode(std::shared_ptr<Value> value)
    : value(std::move(value)) {}

std::shared_ptr<Value> LiteralNode::evaluate(Interpreter &) { return value; }

std::string LiteralNode::toString() const {
    return value ? value->toString() : "null";
}

// VariableNode implementation
VariableNode::VariableNode(const std::string &name) : name(name) {}

const std::string &VariableNode::getName() const { return name; }

std::shared_ptr<Value> VariableNode::evaluate(Interpreter &interpreter) {
    return interpreter.lookUpVariable(name);
}

std::string VariableNode::toString() const { return name; }

// BinaryNode implementation
BinaryNode::BinaryNode(std::shared_ptr<Node> left, const Token &op,
                       std::shared_ptr<Node> right)
    : left(std::move(left)), op(op), right(std::move(right)) {}

std::shared_ptr<Value> BinaryNode::evaluate(Interpreter &interpreter) {
    std::shared_ptr<Value> leftVal = left->evaluate(interpreter);
    std::shared_ptr<Value> rightVal = right->evaluate(interpreter);

    switch (op.type) {
    case TokenType::PLUS:
        if (leftVal->isString() && rightVal->isString()) {
            return std::make_shared<Value>(leftVal->getString() +
                                           rightVal->getString());
        }
        if (leftVal->isNumber() || rightVal->isNumber()) {
            if (leftVal->isInt() && rightVal->isInt()) {
                return std::make_shared<Value>(leftVal->getInt() +
                                               rightVal->getInt());
            }
            return std::make_shared<Value>(leftVal->asDouble() +
                                           rightVal->asDouble());
        }
        throw std::runtime_error("Operands must be numbers or strings.");

    case TokenType::MINUS:
        if (leftVal->isNumber() && rightVal->isNumber()) {
            if (leftVal->isInt() && rightVal->isInt()) {
                return std::make_shared<Value>(leftVal->getInt() -
                                               rightVal->getInt());
            }
            return std::make_shared<Value>(leftVal->asDouble() -
                                           rightVal->asDouble());
        }
        throw std::runtime_error("Operands must be numbers.");

    case TokenType::MULTIPLY:
        if (leftVal->isNumber() && rightVal->isNumber()) {
            if (leftVal->isInt() && rightVal->isInt()) {
                return std::make_shared<Value>(leftVal->getInt() *
                                               rightVal->getInt());
            }
            return std::make_shared<Value>(leftVal->asDouble() *
                                           rightVal->asDouble());
        }
        throw std::runtime_error("Operands must be numbers.");

    case TokenType::DIVIDE:
        if (leftVal->isNumber() && rightVal->isNumber()) {
            // Check for division by zero
            if ((rightVal->isInt() && rightVal->getInt() == 0) ||
                (rightVal->isDouble() && rightVal->getDouble() == 0.0)) {
                throw std::runtime_error("Division by zero.");
            }

            if (leftVal->isInt() && rightVal->isInt() &&
                leftVal->getInt() % rightVal->getInt() == 0) {
                return std::make_shared<Value>(leftVal->getInt() /
                                               rightVal->getInt());
            }
            return std::make_shared<Value>(leftVal->asDouble() /
                                           rightVal->asDouble());
        }
        throw std::runtime_error("Operands must be numbers.");

    default:
        throw std::runtime_error("Unknown operator.");
    }
}

std::string BinaryNode::toString() const {
    std::stringstream ss;
    ss << "(" << left->toString() << " " << op.lexeme << " "
       << right->toString() << ")";
    return ss.str();
}

// AssignNode implementation
AssignNode::AssignNode(const std::string &name, std::shared_ptr<Node> value)
    : name(name), value(std::move(value)) {}

std::shared_ptr<Value> AssignNode::evaluate(Interpreter &interpreter) {
    std::shared_ptr<Value> val = value->evaluate(interpreter);
    interpreter.assign(name, val);
    return val;
}

std::string AssignNode::toString() const {
    return name + " = " + value->toString();
}

// FunctionNode implementation
FunctionNode::FunctionNode(std::string name,
                           std::vector<std::string> parameters,
                           std::shared_ptr<Node> body)
    : name(std::move(name)), parameters(std::move(parameters)),
      body(std::move(body)) {}

std::shared_ptr<Value> FunctionNode::evaluate(Interpreter &interpreter) {
    auto function = std::make_shared<FunctionObject>(
        parameters, body, interpreter.getEnvironment());
    interpreter.define(name, std::make_shared<Value>(function));
    return std::make_shared<Value>();
}

std::string FunctionNode::toString() const {
    std::stringstream ss;
    ss << "function " << name << "(";

    for (size_t i = 0; i < parameters.size(); ++i) {
        if (i > 0)
            ss << ", ";
        ss << parameters[i];
    }

    ss << ") " << body->toString();
    return ss.str();
}

// CallNode implementation
CallNode::CallNode(std::shared_ptr<Node> callee,
                   std::vector<std::shared_ptr<Node>> arguments)
    : callee(std::move(callee)), arguments(std::move(arguments)) {}

std::shared_ptr<Value> CallNode::evaluate(Interpreter &interpreter) {
    std::shared_ptr<Value> calleeValue = callee->evaluate(interpreter);

    if (!calleeValue->isFunction()) {
        throw std::runtime_error("Can only call functions.");
    }

    std::vector<std::shared_ptr<Value>> argumentValues;
    for (const auto &argument : arguments) {
        argumentValues.push_back(argument->evaluate(interpreter));
    }

    auto function = calleeValue->getFunction();

    if (argumentValues.size() != function->parameters.size()) {
        throw std::runtime_error("Expected " +
                                 std::to_string(function->parameters.size()) +
                                 " arguments but got " +
                                 std::to_string(argumentValues.size()) + ".");
    }

    auto environment = std::make_shared<Environment>(function->closure);

    for (size_t i = 0; i < function->parameters.size(); i++) {
        environment->define(function->parameters[i], argumentValues[i]);
    }

    try {
        interpreter.executeBlock(
            dynamic_cast<Block *>(function->body.get())->getStatements(),
            environment);
    } catch (const ReturnValue &returnValue) {
        return returnValue.value;
    }

    return std::make_shared<Value>();
}

std::string CallNode::toString() const {
    std::stringstream ss;
    ss << callee->toString() << "(";

    for (size_t i = 0; i < arguments.size(); ++i) {
        if (i > 0)
            ss << ", ";
        ss << arguments[i]->toString();
    }

    ss << ")";
    return ss.str();
}

// ReturnNode implementation
ReturnNode::ReturnNode(std::shared_ptr<Node> value) : value(std::move(value)) {}

std::shared_ptr<Value> ReturnNode::evaluate(Interpreter &interpreter) {
    std::shared_ptr<Value> returnValue;

    if (value) {
        returnValue = value->evaluate(interpreter);
    } else {
        returnValue = std::make_shared<Value>();
    }

    throw ReturnValue(returnValue);
}

std::string ReturnNode::toString() const {
    if (value) {
        return "return " + value->toString() + ";";
    } else {
        return "return;";
    }
}

} // namespace esla
