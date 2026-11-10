#pragma once

#include <string>
#include <vector>

class Tokenizer {
public:
    // Splits text into lowercase alphanumeric words.
    static std::vector<std::string> tokenize(const std::string& text);
};