#pragma once

#include "Value.h"
#include <memory>

// Forward declarations
class Interpreter;

/**
 * @class Node
 * @brief Abstract base class for all AST nodes.
 *
 * This class provides the interface for all nodes in the abstract syntax tree.
 * Each specific node type will implement the evaluate method to define its
 * runtime behavior.
 */
class Node {
  public:
    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~Node() = default;

    /**
     * @brief Evaluates the node and returns its value.
     * @param interpreter The interpreter that will execute the node.
     * @return The value produced by evaluating the node.
     */
    virtual std::shared_ptr<Value> evaluate(Interpreter &interpreter) = 0;

    /**
     * @brief Returns a string representation of the node (for debugging).
     * @return A string representation of the node.
     */
    virtual std::string toString() const = 0;
};

/**
 * @class ReturnValue
 * @brief Exception class used to handle return statements in functions.
 *
 * This class is used to implement the control flow for return statements.
 * When a return statement is encountered, this exception is thrown with the
 * return value.
 */
struct ReturnValue {
    std::shared_ptr<Value> value;

    /**
     * @brief Constructs a ReturnValue with the given value.
     * @param value The value to return.
     */
    explicit ReturnValue(std::shared_ptr<Value> value)
        : value(std::move(value)) {}
};
