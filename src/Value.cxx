#include <esla/Value.h>
#include <iomanip>
#include <sstream>

namespace esla {

std::string Value::toString() const {
    if (isNull())
        return "null";

    if (isBool()) {
        return getBool() ? "true" : "false";
    }

    if (isInt()) {
        return std::to_string(getInt());
    }

    if (isDouble()) {
        // Format double with proper precision
        std::ostringstream ss;
        ss << std::fixed;

        // Get the double value
        double val = getDouble();

        // Format with appropriate precision (avoiding trailing zeros)
        std::string str = std::to_string(val);
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        if (str.back() == '.') {
            str.pop_back();
        }

        return str;
    }

    if (isString()) {
        return "'" + getString() + "'";
    }

    if (isFunction()) {
        return "<function>";
    }

    return "unknown";
}

// Utility functions for Value operations

/**
 * @brief Create a Value object from a literal string.
 * @param literal The string literal, possibly representing a number or boolean.
 * @return A Value object containing the appropriate type.
 */
std::shared_ptr<Value> createValueFromLiteral(const std::string &literal) {
    // Check if it's a boolean
    if (literal == "true") {
        return std::make_shared<Value>(true);
    } else if (literal == "false") {
        return std::make_shared<Value>(false);
    }

    // Check if it's a number
    try {
        // First try to parse as an integer
        size_t pos;
        int intValue = std::stoi(literal, &pos);

        // If we consumed the entire string, it's an integer
        if (pos == literal.length()) {
            return std::make_shared<Value>(intValue);
        }

        // Otherwise, try to parse as a double
        double doubleValue = std::stod(literal, &pos);

        // If we consumed the entire string, it's a double
        if (pos == literal.length()) {
            return std::make_shared<Value>(doubleValue);
        }
    } catch (const std::exception &) {
        // Not a number, continue
    }

    // If it's enclosed in single quotes, it's a string
    if (literal.length() >= 2 && literal.front() == '\'' &&
        literal.back() == '\'') {
        return std::make_shared<Value>(literal.substr(1, literal.length() - 2));
    }

    // Default to treating it as a string
    return std::make_shared<Value>(literal);
}

/**
 * @brief Add two values together.
 * @param a The first value.
 * @param b The second value.
 * @return The result of the addition.
 * @throws std::runtime_error if the values cannot be added together.
 */
std::shared_ptr<Value> addValues(const std::shared_ptr<Value> &a,
                                 const std::shared_ptr<Value> &b) {
    // String concatenation
    if (a->isString() && b->isString()) {
        return std::make_shared<Value>(a->getString() + b->getString());
    }

    // Number addition
    if (a->isNumber() && b->isNumber()) {
        // If either is a double, return a double
        if (a->isDouble() || b->isDouble()) {
            return std::make_shared<Value>(a->asDouble() + b->asDouble());
        }
        // Otherwise, return an int
        return std::make_shared<Value>(a->getInt() + b->getInt());
    }

    throw std::runtime_error("Cannot add values of these types");
}

/**
 * @brief Subtract one value from another.
 * @param a The first value.
 * @param b The second value.
 * @return The result of the subtraction.
 * @throws std::runtime_error if the values cannot be subtracted.
 */
std::shared_ptr<Value> subtractValues(const std::shared_ptr<Value> &a,
                                      const std::shared_ptr<Value> &b) {
    // Number subtraction
    if (a->isNumber() && b->isNumber()) {
        // If either is a double, return a double
        if (a->isDouble() || b->isDouble()) {
            return std::make_shared<Value>(a->asDouble() - b->asDouble());
        }
        // Otherwise, return an int
        return std::make_shared<Value>(a->getInt() - b->getInt());
    }

    throw std::runtime_error("Cannot subtract values of these types");
}

/**
 * @brief Multiply two values together.
 * @param a The first value.
 * @param b The second value.
 * @return The result of the multiplication.
 * @throws std::runtime_error if the values cannot be multiplied together.
 */
std::shared_ptr<Value> multiplyValues(const std::shared_ptr<Value> &a,
                                      const std::shared_ptr<Value> &b) {
    // Number multiplication
    if (a->isNumber() && b->isNumber()) {
        // If either is a double, return a double
        if (a->isDouble() || b->isDouble()) {
            return std::make_shared<Value>(a->asDouble() * b->asDouble());
        }
        // Otherwise, return an int
        return std::make_shared<Value>(a->getInt() * b->getInt());
    }

    throw std::runtime_error("Cannot multiply values of these types");
}

/**
 * @brief Divide one value by another.
 * @param a The first value.
 * @param b The second value.
 * @return The result of the division.
 * @throws std::runtime_error if the values cannot be divided or if dividing by
 * zero.
 */
std::shared_ptr<Value> divideValues(const std::shared_ptr<Value> &a,
                                    const std::shared_ptr<Value> &b) {
    // Number division
    if (a->isNumber() && b->isNumber()) {
        // Check for division by zero
        if ((b->isInt() && b->getInt() == 0) ||
            (b->isDouble() && b->getDouble() == 0.0)) {
            throw std::runtime_error("Division by zero");
        }

        // If either is a double, or if the division is not even, return a
        // double
        if (a->isDouble() || b->isDouble() ||
            (a->isInt() && b->isInt() && a->getInt() % b->getInt() != 0)) {
            return std::make_shared<Value>(a->asDouble() / b->asDouble());
        }

        // Otherwise, return an int
        return std::make_shared<Value>(a->getInt() / b->getInt());
    }

    throw std::runtime_error("Cannot divide values of these types");
}

} // namespace esla
