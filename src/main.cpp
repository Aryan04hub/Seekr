#include <iostream>

#include "Tokenizer.h"

int main() {
    std::string text = "Hello, World! Hello... C++17 is GREAT; don't stop.";
    std::vector<std::string> tokens = Tokenizer::tokenize(text);

    std::cout << "Token count: " << tokens.size() << "\n";
    for (const std::string& token : tokens) {
        std::cout << "[" << token << "]\n";
    }
    return 0;
}