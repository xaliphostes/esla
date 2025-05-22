#include <esla/EslaEngine.h>

std::string EslaEngine::eval(const std::string &code) {
    return interpreter.evaluateToString(code);
}

void EslaEngine::exec(const std::string &code) { interpreter.run(code); }

void EslaEngine::loadFile(const std::string &filename) {
    interpreter.executeFile(filename);
}
