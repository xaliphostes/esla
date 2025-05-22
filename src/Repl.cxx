#include <esla/Repl.h>
#include <esla/Lexer.h>
#include <esla/Parser.h>
#include <iostream>

namespace esla {

LinenoiseREPL::LinenoiseREPL() {
    historyFile = ".esla_history";

    // Load history
    linenoiseHistoryLoad(historyFile.c_str());

    // Set maximum history length
    linenoiseHistorySetMaxLen(1000);

    // Set up completion callback
    linenoiseSetCompletionCallback(completionCallback);

    // Set up hints callback (optional)
    linenoiseSetHintsCallback(hintsCallback);

    // Enable multi-line mode
    linenoiseSetMultiLine(1);

    // Setup completion words
    setupCompletions();
}

LinenoiseREPL::~LinenoiseREPL() {
    // Save history
    linenoiseHistorySave(historyFile.c_str());
}

void LinenoiseREPL::run() {
    std::cout << "ESLA Interactive Shell with linenoise" << std::endl;
    std::cout << "Cross-platform readline alternative" << std::endl;
    std::cout << "Type 'exit' to quit, 'help' for commands" << std::endl;

    char *line;
    while ((line = linenoise("esla> ")) != nullptr) {
        std::string command(line);

        // Add to history if not empty
        if (!command.empty()) {
            linenoiseHistoryAdd(line);
        }

        // Process the command
        if (!processCommand(command)) {
            free(line);
            break;
        }

        free(line);
    }

    std::cout << "Goodbye!" << std::endl;
}

void LinenoiseREPL::setupCompletions() {
    completions = {// Keywords
                   "function", "return", "if", "else", "while", "for", "true",
                   "false", "null",

                   // REPL commands
                   "exit", "quit", "vars", "history", "clear", "help",

                   // Common functions (if you add them)
                   "print", "typeof", "length"};
}

bool LinenoiseREPL::processCommand(const std::string &line) {
    if (line.empty()) {
        return true;
    }

    if (line == "exit" || line == "quit") {
        return false;
    }

    if (line == "help") {
        printHelp();
        return true;
    }

    if (line == "vars") {
        interpreter.printVariables();
        return true;
    }

    if (line == "history") {
        printHistory();
        return true;
    }

    if (line == "clear") {
        linenoiseClearScreen();
        return true;
    }

    // Execute ESLA code
    try {
        Lexer lexer(line);
        std::vector<Token> tokens = lexer.scanTokens();

        Parser parser(tokens);
        std::shared_ptr<Node> expression = parser.parse();

        if (expression) {
            std::shared_ptr<Value> result = interpreter.evaluate(expression);
            if (result && !result->isNull()) {
                std::cout << "=> ";
                interpreter.printValue(result);
                std::cout << std::endl;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return true;
}

void LinenoiseREPL::printHelp() {
    std::cout << "ESLA Interactive Shell Commands:" << std::endl;
    std::cout << "  exit, quit    - Exit the shell" << std::endl;
    std::cout << "  vars          - List all variables" << std::endl;
    std::cout << "  history       - Show command history" << std::endl;
    std::cout << "  clear         - Clear the screen" << std::endl;
    std::cout << "  help          - Show this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "Keyboard shortcuts:" << std::endl;
    std::cout << "  Up/Down       - Navigate command history" << std::endl;
    std::cout << "  Ctrl+A        - Move to beginning of line" << std::endl;
    std::cout << "  Ctrl+E        - Move to end of line" << std::endl;
    std::cout << "  Ctrl+C        - Cancel current line" << std::endl;
    std::cout << "  Tab           - Auto-complete" << std::endl;
}

void LinenoiseREPL::printHistory() {
    // linenoise doesn't provide direct access to history
    // We'll need to maintain our own history for display
    std::cout << "History is maintained automatically." << std::endl;
    std::cout << "Use Up/Down arrows to navigate through previous commands."
              << std::endl;
}

// Static callback functions for linenoise
void LinenoiseREPL::completionCallback(const char *buf,
                                       linenoiseCompletions *lc) {
    // Get the instance pointer (you'd need to store this globally or use a
    // singleton) For simplicity, we'll use a static completion list

    static std::vector<std::string> keywords = {
        "function", "return", "if",      "else",  "while",
        "for",      "true",   "false",   "null",  "exit",
        "quit",     "vars",   "history", "clear", "help"};

    std::string input(buf);
    for (const auto &keyword : keywords) {
        if (keyword.find(input) == 0) {
            linenoiseAddCompletion(lc, keyword.c_str());
        }
    }
}

char *LinenoiseREPL::hintsCallback(const char *buf, int *color, int *bold) {
    // Provide hints as you type
    std::string input(buf);

    if (input == "exit" || input == "quit") {
        *color = 35; // Magenta
        *bold = 0;
        return strdup(" (exit the shell)");
    }

    if (input == "vars") {
        *color = 35;
        *bold = 0;
        return strdup(" (show variables)");
    }

    if (input.find("function") == 0) {
        *color = 32; // Green
        *bold = 1;
        return strdup(" name(params) { ... }");
    }

    return nullptr;
}

} // namespace esla