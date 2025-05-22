#include <iostream>
#include <string>
#include <esla/ObjectHandle.h>
#include <esla/Value.h>
#include <esla/Interpreter.h>


class Person {
  private:
    std::string name;
    int age;
    std::vector<std::string> hobbies;

  public:
    Person(const std::string &name, int age) : name(name), age(age) {}

    // Getters
    const std::string &getName() const { return name; }
    int getAge() const { return age; }
    const std::vector<std::string> &getHobbies() const { return hobbies; }

    // Setters
    void setName(const std::string &newName) { name = newName; }
    void setAge(int newAge) { age = newAge; }

    // Methods
    void addHobby(const std::string &hobby) { hobbies.push_back(hobby); }

    std::string introduce() const {
        return "Hi, I'm " + name + " and I'm " + std::to_string(age) +
               " years old.";
    }

    void celebrateBirthday() {
        age++;
        std::cout << name << " is now " << age << " years old!" << std::endl;
    }
};

// ---------------------------------------------------------------

// Helper function to create object handles for Person class
std::shared_ptr<esla::ObjectHandle> createPersonHandle(Person *person) {
    auto handle = std::make_shared<esla::ObjectHandle>(person, "Person");

    // Register properties
    handle->registerGetter("name", [](std::any &obj) {
        auto *p = std::any_cast<Person *>(obj);
        return std::make_shared<esla::Value>(p->getName());
    });

    handle->registerSetter(
        "name", [](std::any &obj, std::shared_ptr<esla::Value> value) {
            if (!value->isString()) {
                throw std::runtime_error("name must be a string");
            }
            auto *p = std::any_cast<Person *>(obj);
            p->setName(value->getString());
        });

    handle->registerGetter("age", [](std::any &obj) {
        auto *p = std::any_cast<Person *>(obj);
        return std::make_shared<esla::Value>(p->getAge());
    });

    handle->registerSetter(
        "age", [](std::any &obj, std::shared_ptr<esla::Value> value) {
            if (!value->isInt()) {
                throw std::runtime_error("age must be an integer");
            }
            auto *p = std::any_cast<Person *>(obj);
            p->setAge(value->getInt());
        });

    // Register methods
    handle->registerMethod(
        "introduce",
        [](std::any &obj, const std::vector<std::shared_ptr<esla::Value>> &) {
            auto *p = std::any_cast<Person *>(obj);
            return std::make_shared<esla::Value>(p->introduce());
        });

    handle->registerMethod(
        "addHobby", [](std::any &obj,
                       const std::vector<std::shared_ptr<esla::Value>> &args) {
            if (args.size() != 1 || !args[0]->isString()) {
                throw std::runtime_error(
                    "addHobby expects one string argument");
            }
            auto *p = std::any_cast<Person *>(obj);
            p->addHobby(args[0]->getString());
            return std::make_shared<esla::Value>();
        });

    handle->registerMethod(
        "celebrateBirthday",
        [](std::any &obj, const std::vector<std::shared_ptr<esla::Value>> &) {
            auto *p = std::any_cast<Person *>(obj);
            p->celebrateBirthday();
            return std::make_shared<esla::Value>();
        });

    return handle;
}

int main() {
    // Create C++ objects
    Person alice("Alice", 25);
    Person bob("Bob", 30);

    // Create ESLA interpreter
    esla::Interpreter interpreter;

    // Create object handles and register them in ESLA
    auto aliceHandle = createPersonHandle(&alice);
    auto bobHandle = createPersonHandle(&bob);

    interpreter.define("alice", std::make_shared<esla::Value>(aliceHandle));
    interpreter.define("bob", std::make_shared<esla::Value>(bobHandle));

    // Now you can use them in ESLA scripts!
    try {
        // Test property access
        std::cout << "=== Property Access ===" << std::endl;
        auto result = interpreter.evaluate("alice.name");
        interpreter.printValue(result);
        std::cout << std::endl;

        result = interpreter.evaluate("alice.age");
        interpreter.printValue(result);
        std::cout << std::endl;

        // Test property assignment
        std::cout << "=== Property Assignment ===" << std::endl;
        interpreter.evaluate("alice.name = 'Alice Smith'");
        interpreter.evaluate("alice.age = 26");

        result = interpreter.evaluate("alice.name");
        interpreter.printValue(result);
        std::cout << std::endl;

        result = interpreter.evaluate("alice.age");
        interpreter.printValue(result);
        std::cout << std::endl;

        // Test method calls
        std::cout << "=== Method Calls ===" << std::endl;
        result = interpreter.evaluate("alice.introduce()");
        interpreter.printValue(result);
        std::cout << std::endl;

        interpreter.evaluate("alice.addHobby('reading')");
        interpreter.evaluate("alice.addHobby('coding')");

        // Test birthday celebration
        std::cout << "=== Birthday Celebration ===" << std::endl;
        interpreter.evaluate("alice.celebrateBirthday()");

        result = interpreter.evaluate("alice.age");
        std::cout << "Alice's new age: ";
        interpreter.printValue(result);
        std::cout << std::endl;

        // Test with bob
        std::cout << "=== Working with Bob ===" << std::endl;
        result = interpreter.evaluate("bob.introduce()");
        interpreter.printValue(result);
        std::cout << std::endl;

        interpreter.evaluate("bob.age = 31");
        result = interpreter.evaluate("bob.introduce()");
        interpreter.printValue(result);
        std::cout << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}