#pragma once

#include "Node.h"
#include "Token.h"
#include <memory>
#include <string>
#include <vector>

namespace esla {

// Forward declarations
class Interpreter;
class Environment;

/**
 * @class ExpressionStmt
 * @brief Represents an expression statement.
 *
 * An expression statement is an expression that is evaluated for its side
 * effects, not its value (e.g., a function call or an assignment).
 */
class ExpressionStmt : public Node {
  private:
    std::shared_ptr<Node> expression;

  public:
    /**
     * @brief Constructs an ExpressionStmt with the given expression.
     * @param expression The expression to evaluate.
     */
    ExpressionStmt(std::shared_ptr<Node> expression);

    /**
     * @brief Evaluates the expression statement.
     * @param interpreter The interpreter that will execute the node.
     * @return The value produced by evaluating the expression.
     */
    std::shared_ptr<Value> evaluate(Interpreter &interpreter) override;

    /**
     * @brief Returns a string representation of the expression statement.
     * @return A string representation of the expression statement.
     */
    std::string toString() const override;
};

/**
 * @class Block
 * @brief Represents a block of statements.
 *
 * A block is a sequence of statements enclosed in curly braces. It creates a
 * new scope.
 */
class Block : public Node {
  private:
    std::vector<std::shared_ptr<Node>> statements;

  public:
    /**
     * @brief Constructs a Block with the given statements.
     * @param statements The statements in the block.
     */
    Block(std::vector<std::shared_ptr<Node>> statements);

    /**
     * @brief Evaluates the block.
     * @param interpreter The interpreter that will execute the node.
     * @return The value produced by evaluating the last statement, or null if
     * the block is empty.
     */
    std::shared_ptr<Value> evaluate(Interpreter &interpreter) override;

    /**
     * @brief Returns a string representation of the block.
     * @return A string representation of the block.
     */
    std::string toString() const override;

    /**
     * @brief Gets the statements in this block.
     * @return A reference to the vector of statements.
     */
    const std::vector<std::shared_ptr<Node>> &getStatements() const;
};

/**
 * @class LiteralNode
 * @brief Represents a literal value (number, string, etc.).
 */
class LiteralNode : public Node {
  private:
    std::shared_ptr<Value> value;

  public:
    /**
     * @brief Constructs a LiteralNode with the given value.
     * @param value The literal value.
     */
    LiteralNode(std::shared_ptr<Value> value);

    /**
     * @brief Evaluates the literal node.
     * @param interpreter The interpreter that will execute the node.
     * @return The literal value.
     */
    std::shared_ptr<Value> evaluate(Interpreter &interpreter) override;

    /**
     * @brief Returns a string representation of the literal node.
     * @return A string representation of the literal node.
     */
    std::string toString() const override;
};

/**
 * @class VariableNode
 * @brief Represents a variable reference.
 */
class VariableNode : public Node {
  private:
    std::string name;

  public:
    /**
     * @brief Constructs a VariableNode with the given name.
     * @param name The variable name.
     */
    VariableNode(const std::string &name);

    /**
     * @brief Gets the variable name.
     * @return The variable name.
     */
    const std::string &getName() const;

    /**
     * @brief Evaluates the variable node.
     * @param interpreter The interpreter that will execute the node.
     * @return The value of the variable.
     */
    std::shared_ptr<Value> evaluate(Interpreter &interpreter) override;

    /**
     * @brief Returns a string representation of the variable node.
     * @return A string representation of the variable node.
     */
    std::string toString() const override;
};

/**
 * @class BinaryNode
 * @brief Represents a binary operation (e.g., a + b).
 */
class BinaryNode : public Node {
  private:
    std::shared_ptr<Node> left;
    Token op;
    std::shared_ptr<Node> right;

  public:
    /**
     * @brief Constructs a BinaryNode with the given operands and operator.
     * @param left The left operand.
     * @param op The operator token.
     * @param right The right operand.
     */
    BinaryNode(std::shared_ptr<Node> left, const Token &op,
               std::shared_ptr<Node> right);

    /**
     * @brief Evaluates the binary node.
     * @param interpreter The interpreter that will execute the node.
     * @return The result of the binary operation.
     */
    std::shared_ptr<Value> evaluate(Interpreter &interpreter) override;

    /**
     * @brief Returns a string representation of the binary node.
     * @return A string representation of the binary node.
     */
    std::string toString() const override;
};

/**
 * @class AssignNode
 * @brief Represents a variable assignment (e.g., a = b).
 */
class AssignNode : public Node {
  private:
    std::string name;
    std::shared_ptr<Node> value;

  public:
    /**
     * @brief Constructs an AssignNode with the given variable name and value.
     * @param name The variable name.
     * @param value The value to assign.
     */
    AssignNode(const std::string &name, std::shared_ptr<Node> value);

    /**
     * @brief Evaluates the assignment node.
     * @param interpreter The interpreter that will execute the node.
     * @return The assigned value.
     */
    std::shared_ptr<Value> evaluate(Interpreter &interpreter) override;

    /**
     * @brief Returns a string representation of the assignment node.
     * @return A string representation of the assignment node.
     */
    std::string toString() const override;
};

/**
 * @class FunctionNode
 * @brief Represents a function declaration.
 */
class FunctionNode : public Node {
  private:
    std::string name;
    std::vector<std::string> parameters;
    std::shared_ptr<Node> body;

  public:
    /**
     * @brief Constructs a FunctionNode with the given name, parameters, and
     * body.
     * @param name The function name.
     * @param parameters The function parameters.
     * @param body The function body.
     */
    FunctionNode(std::string name, std::vector<std::string> parameters,
                 std::shared_ptr<Node> body);

    /**
     * @brief Evaluates the function declaration.
     * @param interpreter The interpreter that will execute the node.
     * @return Null value (function declarations don't produce values).
     */
    std::shared_ptr<Value> evaluate(Interpreter &interpreter) override;

    /**
     * @brief Returns a string representation of the function declaration.
     * @return A string representation of the function declaration.
     */
    std::string toString() const override;
};

/**
 * @class CallNode
 * @brief Represents a function call.
 */
class CallNode : public Node {
  private:
    std::shared_ptr<Node> callee;
    std::vector<std::shared_ptr<Node>> arguments;

  public:
    /**
     * @brief Constructs a CallNode with the given callee and arguments.
     * @param callee The function to call.
     * @param arguments The function arguments.
     */
    CallNode(std::shared_ptr<Node> callee,
             std::vector<std::shared_ptr<Node>> arguments);

    /**
     * @brief Evaluates the function call.
     * @param interpreter The interpreter that will execute the node.
     * @return The value returned by the function.
     */
    std::shared_ptr<Value> evaluate(Interpreter &interpreter) override;

    /**
     * @brief Returns a string representation of the function call.
     * @return A string representation of the function call.
     */
    std::string toString() const override;
};

/**
 * @class ReturnNode
 * @brief Represents a return statement.
 */
class ReturnNode : public Node {
  private:
    std::shared_ptr<Node> value;

  public:
    /**
     * @brief Constructs a ReturnNode with the given value.
     * @param value The value to return, or nullptr for a void return.
     */
    ReturnNode(std::shared_ptr<Node> value);

    /**
     * @brief Evaluates the return statement.
     * @param interpreter The interpreter that will execute the node.
     * @return This method never returns normally; it always throws a
     * ReturnValue exception.
     */
    std::shared_ptr<Value> evaluate(Interpreter &interpreter) override;

    /**
     * @brief Returns a string representation of the return statement.
     * @return A string representation of the return statement.
     */
    std::string toString() const override;
};

} // namespace esla
