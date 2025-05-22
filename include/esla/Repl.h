#pragma once
#include "third/linenoise.h"
#include <esla/Interpreter.h>
#include <esla/Value.h>

namespace esla {

class LinenoiseREPL {
  public:
    LinenoiseREPL();
    ~LinenoiseREPL();

    void run();

  private:
    void setupCompletions();
    bool processCommand(const std::string &line);
    void printHelp();
    void printHistory();

    // Static callback functions for linenoise
    static void completionCallback(const char *buf, linenoiseCompletions *lc);
    static char *hintsCallback(const char *buf, int *color, int *bold);

  private:
    Interpreter interpreter;
    std::string historyFile;
    std::vector<std::string> completions;
};

} // namespace esla