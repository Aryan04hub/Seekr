#include "Trie.h"

Trie::Trie() : root_(std::make_unique<Node>()) {}

void Trie::insert(const std::string& word) {
    Node* node = root_.get();
    for (char ch : word) {
        std::unique_ptr<Node>& child = node->children[ch];
        if (!child) {
            child = std::make_unique<Node>();
        }
        node = child.get();
    }
    node->isEndOfWord = true;
}

const Trie::Node* Trie::findNode(const std::string& prefix) const {
    const Node* node = root_.get();
    for (char ch : prefix) {
        auto it = node->children.find(ch);
        if (it == node->children.end()) {
            return nullptr;
        }
        node = it->second.get();
    }
    return node;
}

bool Trie::contains(const std::string& word) const {
    const Node* node = findNode(word);
    return node != nullptr && node->isEndOfWord;
}

std::vector<std::string> Trie::suggest(const std::string& prefix, std::size_t limit) const {
    std::vector<std::string> results;
    if (limit == 0) {
        return results;
    }
    const Node* node = findNode(prefix);
    if (node == nullptr) {
        return results;
    }
    std::string current = prefix;
    collect(node, current, results, limit);
    return results;
}

void Trie::collect(const Node* node, std::string& current,
                   std::vector<std::string>& out, std::size_t limit) const {
    if (out.size() >= limit) {
        return;
    }
    if (node->isEndOfWord) {
        out.push_back(current);
    }
    for (const auto& entry : node->children) {
        current.push_back(entry.first);
        collect(entry.second.get(), current, out, limit);
        current.pop_back();
        if (out.size() >= limit) {
            return;
        }
    }
}