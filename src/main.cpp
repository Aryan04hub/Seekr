#include <iostream>
#include <string>
#include <vector>

#include "Trie.h"

void showSuggestions(const Trie& trie, const std::string& prefix) {
    std::cout << "Prefix \"" << prefix << "\" -> ";
    std::vector<std::string> words = trie.suggest(prefix, 5);
    if (words.empty()) {
        std::cout << "(no suggestions)";
    }
    for (const std::string& w : words) {
        std::cout << w << " ";
    }
    std::cout << "\n";
}

int main() {
    Trie trie;
    for (const std::string& w : {"index", "indexing", "indexes", "search",
                                 "searching", "engine", "trie", "tree"}) {
        trie.insert(w);
    }

    showSuggestions(trie, "ind");
    showSuggestions(trie, "sea");
    showSuggestions(trie, "tr");
    showSuggestions(trie, "x");
    showSuggestions(trie, "");

    std::cout << "contains(\"index\")  = " << trie.contains("index") << "\n";
    std::cout << "contains(\"inde\")   = " << trie.contains("inde") << "\n";
    return 0;
}