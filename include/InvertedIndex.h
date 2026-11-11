#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Document.h"

class InvertedIndex {
public:
    // docId -> term frequency of a word in that document
    using Postings = std::unordered_map<int, int>;

    // Tokenizes the document and adds every word to the index.
    void addDocument(const Document& doc);

    // Returns the postings of a word, or nullptr if the word is not indexed.
    const Postings* getPostings(const std::string& word) const;

    std::size_t documentCount() const;
    std::size_t vocabularySize() const;

        // Returns every indexed word (used to build the autocomplete Trie).
    std::vector<std::string> vocabulary() const;

    // Returns the document with this ID, or nullptr if the ID is invalid.
    const Document* getDocument(int id) const;

private:
    std::unordered_map<std::string, Postings> index_;
    std::vector<Document> documents_;
};