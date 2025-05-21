# ESLA: Embedded Scripting Language

![C++](https://img.shields.io/badge/c++-%2300599C.svg)
[![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg)](https://github.com/username/esla/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Version](https://img.shields.io/badge/version-0.1.0-green.svg)](https://semver.org)

ESLA (Embedded Scripting Language) is a lightweight, embeddable scripting language designed for integration into C++ applications. It features a simple syntax, intuitive semantics, and a clean C++ API.

## Features

- **Lightweight**: Small footprint with minimal dependencies
- **Embeddable**: Easy to integrate into existing C++ applications
- **Modern C++**: Built with C++17 for robust memory management and type safety
- **Dynamic Typing**: Variables are dynamically typed for flexibility
- **Function Support**: First-class functions with closures
- **Extensible**: Easy to extend with new features and native functions

## Language Overview

ESLA's syntax is designed to be familiar to developers who know JavaScript, Python, or other modern scripting languages.

### Basic Syntax

```javascript
// Variable assignment
a = 42
b = 3.14
s = 'Hello, world!'
flag = true

// Arithmetic operations
sum = a + b
difference = a - b
product = a * b
quotient = a / b

// String concatenation
greeting = 'Hello, ' + 'ESLA!'
```

### Functions

```javascript
// Function definition
function add(x, y) {
    return x + y
}

// Function calls
result = add(5, 3)

// Nested functions with closures
function createMultiplier(factor) {
    function multiply(x) {
        return x * factor
    }
    return multiply
}

doubler = createMultiplier(2)
value = doubler(5)  // Returns 10
```

### Advanced Examples

```javascript
// Fibonacci function
function fibonacci(n) {
    if (n <= 1) return n
    return fibonacci(n - 1) + fibonacci(n - 2)
}

// Calculation
fib5 = fibonacci(5)  // Returns 5

// String operations
message = 'The result is: ' + fib5
```

## Building ESLA

### Prerequisites

- CMake 3.12 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)

### Build Steps

```bash
# Clone the repository
git clone https://github.com/username/esla.git
cd esla

# Create a build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .

# Run tests
cmake --build . --target run-test
```

For detailed build instructions, see the [CMake Build Guide](cmake-usage-guide.md).

## Integrating ESLA Into Your Applications

### Using CMake

```cmake
# In your CMakeLists.txt
find_package(ESLA REQUIRED)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE ESLA::esla)
```

### Sample Code

```cpp
#include <esla/Interpreter.h>
#include <iostream>
#include <string>

int main() {
    // Create interpreter
    esla::Interpreter interpreter;
    
    // Execute code
    try {
        // Define a variable in ESLA
        interpreter.evaluate("x = 42;");
        
        // Define a function
        interpreter.evaluate(
            "function greet(name) {"
            "    return 'Hello, ' + name + '!';"
            "}"
        );
        
        // Call a function and get the result
        auto result = interpreter.evaluate("greet('World')");
        
        // Print the result
        std::cout << "Result: " << result->toString() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

## Extending ESLA

ESLA can be extended with native C++ functions:

```cpp
#include <esla/Interpreter.h>
#include <esla/Value.h>
#include <iostream>

// Define a native function
std::shared_ptr<esla::Value> print(
    esla::Interpreter& interpreter,
    const std::vector<std::shared_ptr<esla::Value>>& args
) {
    for (const auto& arg : args) {
        std::cout << arg->toString();
    }
    std::cout << std::endl;
    return std::make_shared<esla::Value>();  // Return null
}

int main() {
    esla::Interpreter interpreter;
    
    // Register the native function
    interpreter.registerNativeFunction("print", print);
    
    // Use the native function in ESLA
    interpreter.evaluate("print('Hello from native function!')");
    
    return 0;
}
```

## Roadmap

- **0.2.0**: Add control flow statements (if/else, while, for)
- **0.3.0**: Add standard library (math, string, file I/O)
- **0.4.0**: Support for modules and imports
- **0.5.0**: Add object system with properties and methods
- **1.0.0**: Full language stabilization and documentation

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create a feature branch: `git checkout -b feature-name`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin feature-name`
5. Submit a pull request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspired by languages like JavaScript, Lua, and Python
- Built using modern C++ techniques and best practices
- Uses recursive descent parsing for clean, maintainable code

---

For more information, API documentation, and examples, visit the [ESLA Documentation](https://xaliphostes.github.io/esla/).