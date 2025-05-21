#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <variant>

// Forward declarations
struct FunctionObject;

/**
 * @class Value
 * @brief Represents a value in the scripting language.
 *
 * This class can represent various data types including null, boolean, integer,
 * double, string, and function references. It provides type checking and
 * conversion methods.
 */
class Value {
  public:
    /**
     * @brief Variant type that can hold any of the supported data types.
     */
    using Variant = std::variant<std::monostate, bool, int, double, std::string,
                                 std::shared_ptr<FunctionObject>>;

  private:
    Variant data;

  public:
    /**
     * @brief Default constructor. Creates a null value.
     */
    Value() : data(std::monostate{}) {}

    /**
     * @brief Constructs a boolean value.
     * @param value The boolean value.
     */
    Value(bool value) : data(value) {}

    /**
     * @brief Constructs an integer value.
     * @param value The integer value.
     */
    Value(int value) : data(value) {}

    /**
     * @brief Constructs a double value.
     * @param value The double value.
     */
    Value(double value) : data(value) {}

    /**
     * @brief Constructs a string value.
     * @param value The string value.
     */
    Value(const std::string &value) : data(value) {}

    /**
     * @brief Constructs a function value.
     * @param func The function object.
     */
    Value(std::shared_ptr<FunctionObject> func) : data(std::move(func)) {}

    /**
     * @brief Checks if the value is null.
     * @return true if the value is null, false otherwise.
     */
    bool isNull() const { return std::holds_alternative<std::monostate>(data); }

    /**
     * @brief Checks if the value is a boolean.
     * @return true if the value is a boolean, false otherwise.
     */
    bool isBool() const { return std::holds_alternative<bool>(data); }

    /**
     * @brief Checks if the value is an integer.
     * @return true if the value is an integer, false otherwise.
     */
    bool isInt() const { return std::holds_alternative<int>(data); }

    /**
     * @brief Checks if the value is a double.
     * @return true if the value is a double, false otherwise.
     */
    bool isDouble() const { return std::holds_alternative<double>(data); }

    /**
     * @brief Checks if the value is a string.
     * @return true if the value is a string, false otherwise.
     */
    bool isString() const { return std::holds_alternative<std::string>(data); }

    /**
     * @brief Checks if the value is a function.
     * @return true if the value is a function, false otherwise.
     */
    bool isFunction() const {
        return std::holds_alternative<std::shared_ptr<FunctionObject>>(data);
    }

    /**
     * @brief Checks if the value is a number (either integer or double).
     * @return true if the value is a number, false otherwise.
     */
    bool isNumber() const { return isInt() || isDouble(); }

    /**
     * @brief Gets the boolean value.
     * @return The boolean value.
     * @throws std::runtime_error if the value is not a boolean.
     */
    bool getBool() const {
        if (!isBool())
            throw std::runtime_error("Value is not a boolean");
        return std::get<bool>(data);
    }

    /**
     * @brief Gets the integer value.
     * @return The integer value.
     * @throws std::runtime_error if the value is not an integer.
     */
    int getInt() const {
        if (!isInt())
            throw std::runtime_error("Value is not an integer");
        return std::get<int>(data);
    }

    /**
     * @brief Gets the double value.
     * @return The double value.
     * @throws std::runtime_error if the value is not a double.
     */
    double getDouble() const {
        if (!isDouble())
            throw std::runtime_error("Value is not a double");
        return std::get<double>(data);
    }

    /**
     * @brief Gets the string value.
     * @return The string value.
     * @throws std::runtime_error if the value is not a string.
     */
    const std::string &getString() const {
        if (!isString())
            throw std::runtime_error("Value is not a string");
        return std::get<std::string>(data);
    }

    /**
     * @brief Gets the function object.
     * @return The function object.
     * @throws std::runtime_error if the value is not a function.
     */
    std::shared_ptr<FunctionObject> getFunction() const {
        if (!isFunction())
            throw std::runtime_error("Value is not a function");
        return std::get<std::shared_ptr<FunctionObject>>(data);
    }

    /**
     * @brief Converts the value to a double if possible.
     * @return The value as a double.
     * @throws std::runtime_error if the value cannot be converted to a double.
     */
    double asDouble() const {
        if (isInt())
            return static_cast<double>(getInt());
        if (isDouble())
            return getDouble();
        if (isBool())
            return getBool() ? 1.0 : 0.0;
        throw std::runtime_error("Cannot convert to number");
    }

    /**
     * @brief Converts the value to a string representation.
     * @return The string representation of the value.
     */
    std::string toString() const;

    /**
     * @brief Compares two values for equality.
     * @param other The value to compare with.
     * @return true if the values are equal, false otherwise.
     */
    bool operator==(const Value &other) const { return data == other.data; }

    /**
     * @brief Gets the underlying variant.
     * @return The variant containing the value.
     */
    const Variant &getVariant() const { return data; }
};

/**
 * @struct FunctionObject
 * @brief Represents a function in the scripting language.
 *
 * This struct stores the function's parameters, body, and closure environment.
 */
struct FunctionObject {
    std::vector<std::string> parameters;
    std::shared_ptr<class Node> body;
    std::shared_ptr<class Environment> closure;

    /**
     * @brief Constructs a function object.
     * @param params The function parameters.
     * @param body The function body.
     * @param closure The closure environment.
     */
    FunctionObject(std::vector<std::string> params,
                   std::shared_ptr<class Node> body,
                   std::shared_ptr<class Environment> closure)
        : parameters(std::move(params)), body(std::move(body)),
          closure(std::move(closure)) {}
};
