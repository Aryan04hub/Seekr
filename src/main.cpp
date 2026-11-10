#include <iostream>
#include <string>
#include <vector>

#include "DocumentReader.h"
#include "InvertedIndex.h"

int main(int argc, char* argv[]) {
    std::string directory = (argc > 1) ? argv[1] : "data";

    InvertedIndex index;
    for (const Document& doc : DocumentReader::readDirectory(directory)) {
        index.addDocument(doc);
    }

    std::cout << "Documents indexed: " << index.documentCount() << "\n";
    std::cout << "Unique words: " << index.vocabularySize() << "\n\n";

    std::vector<std::string> testWords = {"index", "c", "words", "banana"};
    for (const std::string& word : testWords) {
        std::cout << "\"" << word << "\" -> ";
        const InvertedIndex::Postings* postings = index.getPostings(word);
        if (postings == nullptr) {
            std::cout << "not found\n";
            continue;
        }
        for (const auto& entry : *postings) {
            std::cout << index.getDocument(entry.first)->getName()
                      << " (freq " << entry.second << ")  ";
        }
        std::cout << "\n";
    }
    return 0;
}