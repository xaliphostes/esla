#include <scripter/Interpreter.h>
#include <scripter/Lexer.h>
#include <scripter/Parser.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/**
 * @brief Runs the given script file.
 * @param path The path to the script file.
 * @return 0 on success, non-zero on error.
 */
int runFile(const std::string &path) {
    // Read the file
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    // Create lexer, parser, and interpreter
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    Parser parser(tokens);
    auto statements = parser.parseAll();

    if (statements.empty()) {
        std::cerr << "Failed to parse file." << std::endl;
        return 1;
    }

    Interpreter interpreter;

    try {
        for (const auto &statement : statements) {
            interpreter.evaluate(statement);
        }
    } catch (const std::exception &e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown runtime error." << std::endl;
        return 1;
    }

    return 0;
}

/**
 * @brief Runs an interactive REPL for the scripting language.
 */
void runRepl() {
    Interpreter interpreter;
    std::string line;

    std::cout << "Simple Scripting Language (type 'exit' to quit, 'vars' to "
                 "list variables)"
              << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);

        if (line == "exit")
            break;

        if (line == "vars") {
            interpreter.printVariables();
            continue;
        }

        if (line.empty())
            continue;

        try {
            Lexer lexer(line);
            std::vector<Token> tokens = lexer.scanTokens();

            Parser parser(tokens);
            std::shared_ptr<Node> expression = parser.parse();

            if (expression) {
                std::shared_ptr<Value> result =
                    interpreter.evaluate(expression);
                std::cout << "=> ";
                interpreter.printValue(result);
                std::cout << std::endl;
            }
        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

/**
 * @brief Main entry point for the scripting language.
 * @param argc Number of command-line arguments.
 * @param argv Command-line arguments.
 * @return Exit code.
 */
int main(int argc, char *argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " [script_file]" << std::endl;
        return 1;
    }

    if (argc == 2) {
        // Run the script file
        return runFile(argv[1]);
    } else {
        // Run the REPL
        runRepl();
        return 0;
    }
}