#pragma once
#include <esla/Interpreter.h>

/**
 * @example
 * EslaEngine engine;
 * 
 * // Set variables from C++
 * engine.setVariable("x", 42);
 * engine.setVariable("message", std::string("Hello from C++"));
 * 
 * // Execute ESLA code
 * engine.exec("y = x * 2");
 * engine.exec("full_message = message + '!'");
 * 
 * // Get results back to C++
 * int y = engine.getVariable<int>("y");
 * std::string full_msg = engine.getVariable<std::string>("full_message");
 * 
 * std::cout << "y = " << y << std::endl;
 * std::cout << "full_message = " << full_msg << std::endl;
 * 
 * // Evaluate expressions
 * std::string result = engine.eval("x + y");
 * std::cout << "x + y = " << result << std::endl;
 */
class EslaEngine {
  public:
    // Convenience methods for common operations
    template <typename T> void setVariable(const std::string &name, T value) {
        interpreter.define(name, std::make_shared<esla::Value>(value));
    }

    template <typename T> T getVariable(const std::string &name) {
        auto value = interpreter.lookUpVariable(name);
        if constexpr (std::is_same_v<T, int>) {
            return value->getInt();
        } else if constexpr (std::is_same_v<T, double>) {
            return value->asDouble();
        } else if constexpr (std::is_same_v<T, std::string>) {
            return value->getString();
        } else if constexpr (std::is_same_v<T, bool>) {
            return value->getBool();
        }
    }

    template <typename T>
    void registerObject(const std::string &name, T *object) {
        // This would use the object registration system from the previous
        // example
        auto handle = createObjectHandle(object);
        interpreter.define(name, std::make_shared<esla::Value>(handle));
    }

  private:
    esla::Interpreter interpreter;
};