#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace esla {

class Value;

/**
 * @class Environment
 * @brief Stores variables and their values in a specific scope.
 *
 * An Environment maintains a mapping from variable names to their values,
 * with support for nested scopes through the enclosing environment reference.
 */
class Environment {
  private:
    std::unordered_map<std::string, std::shared_ptr<Value>> values;
    std::shared_ptr<Environment> enclosing;

  public:
    /**
     * @brief Constructs a global environment.
     */
    Environment();

    /**
     * @brief Constructs a local environment with an enclosing environment.
     * @param enclosing The enclosing environment.
     */
    Environment(std::shared_ptr<Environment> enclosing);

    /**
     * @brief Defines a new variable in the current environment.
     * @param name The variable name.
     * @param value The variable value.
     */
    void define(const std::string &name, std::shared_ptr<Value> value);

    /**
     * @brief Gets the value of a variable.
     * @param name The variable name.
     * @return The variable value.
     * @throws std::runtime_error if the variable is not defined.
     */
    std::shared_ptr<Value> get(const std::string &name);

    /**
     * @brief Assigns a new value to an existing variable.
     * @param name The variable name.
     * @param value The new value.
     * @throws std::runtime_error if the variable is not defined.
     */
    void assign(const std::string &name, std::shared_ptr<Value> value);

    /**
     * @brief Gets all variables in this environment.
     * @return A reference to the map of variable names to values.
     */
    std::unordered_map<std::string, std::shared_ptr<Value>> &getValues();
};

} // namespace esla
