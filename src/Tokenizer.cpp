#include "Tokenizer.h"

#include <cctype>

std::vector<std::string> Tokenizer::tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::string current;

    for (char ch : text) {
        unsigned char uc = static_cast<unsigned char>(ch);
        if (std::isalnum(uc)) {
            current.push_back(static_cast<char>(std::tolower(uc)));
        } else if (!current.empty()) {
            tokens.push_back(current);
            current.clear();
        }
    }

    // Don't lose the last word if the text doesn't end with punctuation
    if (!current.empty()) {
        tokens.push_back(current);
    }

    return tokens;
}