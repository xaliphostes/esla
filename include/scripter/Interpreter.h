#pragma once

#include "Node.h"
#include "AST.h"
#include "Value.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Environment;

/**
 * @class Interpreter
 * @brief Executes the AST produced by the parser.
 *
 * The Interpreter evaluates the AST nodes, executes statements, and manages
 * the execution environment. It handles variable lookup, assignment, function
 * calls, and return values.
 */
class Interpreter {
  private:
    std::shared_ptr<Environment> environment;
    std::shared_ptr<Environment> globals;

    // Helper functions for operations
    std::shared_ptr<Value> add(const std::shared_ptr<Value> &a,
                               const std::shared_ptr<Value> &b);
    std::shared_ptr<Value> subtract(const std::shared_ptr<Value> &a,
                                    const std::shared_ptr<Value> &b);
    std::shared_ptr<Value> multiply(const std::shared_ptr<Value> &a,
                                    const std::shared_ptr<Value> &b);
    std::shared_ptr<Value> divide(const std::shared_ptr<Value> &a,
                                  const std::shared_ptr<Value> &b);

  public:
    /**
     * @brief Constructs an Interpreter with a global environment.
     */
    Interpreter();

    /**
     * @brief Evaluates a node.
     * @param node The node to evaluate.
     * @return The value produced by evaluating the node.
     */
    std::shared_ptr<Value> evaluate(std::shared_ptr<Node> node);

    /**
     * @brief Executes a block of statements in a new environment.
     * @param statements The statements to execute.
     * @param environment The environment to use.
     * @return The value produced by the last statement, or null if the block is
     * empty.
     */
    std::shared_ptr<Value>
    executeBlock(const std::vector<std::shared_ptr<Node>> &statements,
                 std::shared_ptr<Environment> environment);

    /**
     * @brief Looks up the value of a variable.
     * @param name The variable name.
     * @return The variable value.
     * @throws std::runtime_error if the variable is not defined.
     */
    std::shared_ptr<Value> lookUpVariable(const std::string &name);

    /**
     * @brief Assigns a new value to a variable.
     * @param name The variable name.
     * @param value The new value.
     */
    void assign(const std::string &name, std::shared_ptr<Value> value);

    /**
     * @brief Defines a new variable in the current environment.
     * @param name The variable name.
     * @param value The variable value.
     */
    void define(const std::string &name, std::shared_ptr<Value> value);

    /**
     * @brief Gets the current environment.
     * @return The current environment.
     */
    std::shared_ptr<Environment> getEnvironment();

    /**
     * @brief Gets the global environment.
     * @return The global environment.
     */
    std::shared_ptr<Environment> getGlobals();

    /**
     * @brief Prints a value in a readable format.
     * @param value The value to print.
     */
    void printValue(const std::shared_ptr<Value> &value);

    /**
     * @brief Prints all variables in the global environment.
     */
    void printVariables();

    // Friend declarations for AST node evaluators
    friend std::shared_ptr<Value>
    BinaryNode::evaluate(Interpreter &interpreter);
    friend std::shared_ptr<Value>
    VariableNode::evaluate(Interpreter &interpreter);
    friend std::shared_ptr<Value>
    AssignNode::evaluate(Interpreter &interpreter);
    friend std::shared_ptr<Value>
    FunctionNode::evaluate(Interpreter &interpreter);
    friend std::shared_ptr<Value> CallNode::evaluate(Interpreter &interpreter);
    friend std::shared_ptr<Value>
    ReturnNode::evaluate(Interpreter &interpreter);
    friend std::shared_ptr<Value> Block::evaluate(Interpreter &interpreter);
    friend std::shared_ptr<Value>
    ExpressionStmt::evaluate(Interpreter &interpreter);
    friend std::shared_ptr<Value>
    LiteralNode::evaluate(Interpreter &interpreter);
};
