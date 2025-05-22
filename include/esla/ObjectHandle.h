#pragma once

#include <string>
#include <any>
#include "Value.h"

namespace esla {

class ObjectHandle {
  private:
    std::any object;
    std::string typeName;
    std::unordered_map<
        std::string,
        std::function<std::shared_ptr<Value>(
            std::any &, const std::vector<std::shared_ptr<Value>> &)>>
        methods;
    std::unordered_map<std::string,
                       std::function<std::shared_ptr<Value>(std::any &)>>
        getters;
    std::unordered_map<std::string,
                       std::function<void(std::any &, std::shared_ptr<Value>)>>
        setters;

  public:
    template <typename T>
    ObjectHandle(T *obj, const std::string &type)
        : object(obj), typeName(type) {}

    const std::string &getTypeName() const { return typeName; }

    template <typename F>
    void registerMethod(const std::string &name, F &&func) {
        methods[name] = std::forward<F>(func);
    }

    template <typename F>
    void registerGetter(const std::string &name, F &&func) {
        getters[name] = std::forward<F>(func);
    }

    template <typename F>
    void registerSetter(const std::string &name, F &&func) {
        setters[name] = std::forward<F>(func);
    }

    std::shared_ptr<Value>
    callMethod(const std::string &name,
               const std::vector<std::shared_ptr<Value>> &args) {
        auto it = methods.find(name);
        if (it != methods.end()) {
            return it->second(object, args);
        }
        throw std::runtime_error("Method '" + name + "' not found");
    }

    std::shared_ptr<Value> getProperty(const std::string &name) {
        auto it = getters.find(name);
        if (it != getters.end()) {
            return it->second(object);
        }
        throw std::runtime_error("Property '" + name + "' not found");
    }

    void setProperty(const std::string &name, std::shared_ptr<Value> value) {
        auto it = setters.find(name);
        if (it != setters.end()) {
            it->second(object, value);
            return;
        }
        throw std::runtime_error("Property '" + name + "' not found");
    }

    template <typename T> T *getAs() { return std::any_cast<T *>(object); }
};

} // namespace esla