#include <scripter/Environment.h>
#include <scripter/AST.h>
#include <iostream>
#include <stdexcept>

// Environment implementation
Environment::Environment() : enclosing(nullptr) {}

Environment::Environment(std::shared_ptr<Environment> enclosing)
    : enclosing(std::move(enclosing)) {}

void Environment::define(const std::string &name,
                         std::shared_ptr<Value> value) {
    values[name] = std::move(value);
}

std::shared_ptr<Value> Environment::get(const std::string &name) {
    auto it = values.find(name);
    if (it != values.end()) {
        return it->second;
    }

    if (enclosing) {
        return enclosing->get(name);
    }

    throw std::runtime_error("Undefined variable '" + name + "'.");
}

void Environment::assign(const std::string &name,
                         std::shared_ptr<Value> value) {
    auto it = values.find(name);
    if (it != values.end()) {
        it->second = std::move(value);
        return;
    }

    if (enclosing) {
        enclosing->assign(name, std::move(value));
        return;
    }

    throw std::runtime_error("Undefined variable '" + name + "'.");
}

std::unordered_map<std::string, std::shared_ptr<Value>> &
Environment::getValues() {
    return values;
}
