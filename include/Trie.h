#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

class Trie {
public:
    Trie();

    // Adds a word to the trie.
    void insert(const std::string& word);

    // Returns true if the exact word was inserted.
    bool contains(const std::string& word) const;

    // Returns up to `limit` words starting with `prefix`, alphabetically.
    std::vector<std::string> suggest(const std::string& prefix, std::size_t limit) const;

private:
    struct Node {
        std::map<char, std::unique_ptr<Node>> children;
        bool isEndOfWord = false;
    };

    const Node* findNode(const std::string& prefix) const;
    void collect(const Node* node, std::string& current,
                 std::vector<std::string>& out, std::size_t limit) const;

    std::unique_ptr<Node> root_;
};