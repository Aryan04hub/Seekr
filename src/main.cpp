#include <iostream>
#include <string>
#include <vector>

#include "DocumentReader.h"
#include "InvertedIndex.h"
#include "SearchEngine.h"

void runQuery(const SearchEngine& engine, const InvertedIndex& index,
              const std::string& query, QueryMode mode) {
    std::cout << "Query: \"" << query << "\" ["
              << (mode == QueryMode::AND ? "AND" : "OR") << "]\n";

    std::vector<int> ids = engine.search(query, mode);
    if (ids.empty()) {
        std::cout << "  No results.\n";
    }
    for (int id : ids) {
        std::cout << "  - " << index.getDocument(id)->getName() << "\n";
    }
}

int main(int argc, char* argv[]) {
    std::string directory = (argc > 1) ? argv[1] : "data";

    InvertedIndex index;
    for (const Document& doc : DocumentReader::readDirectory(directory)) {
        index.addDocument(doc);
    }

    SearchEngine engine(index);

    runQuery(engine, index, "index", QueryMode::AND);
    runQuery(engine, index, "words documents", QueryMode::OR);
    runQuery(engine, index, "search index", QueryMode::AND);
    runQuery(engine, index, "trie index", QueryMode::AND);
    runQuery(engine, index, "trie index", QueryMode::OR);
    runQuery(engine, index, "banana", QueryMode::OR);
    runQuery(engine, index, "", QueryMode::OR);
    return 0;
}