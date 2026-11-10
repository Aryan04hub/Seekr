#include "InvertedIndex.h"

#include "Tokenizer.h"

void InvertedIndex::addDocument(const Document& doc) {
    documents_.push_back(doc);

    for (const std::string& word : Tokenizer::tokenize(doc.getContent())) {
        index_[word][doc.getId()]++;
    }
}

const InvertedIndex::Postings* InvertedIndex::getPostings(const std::string& word) const {
    auto it = index_.find(word);
    if (it == index_.end()) {
        return nullptr;
    }
    return &it->second;
}

std::size_t InvertedIndex::documentCount() const { return documents_.size(); }

std::size_t InvertedIndex::vocabularySize() const { return index_.size(); }

const Document* InvertedIndex::getDocument(int id) const {
    if (id < 0 || static_cast<std::size_t>(id) >= documents_.size()) {
        return nullptr;
    }
    return &documents_[static_cast<std::size_t>(id)];
}