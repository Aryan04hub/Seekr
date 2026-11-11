#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "DocumentReader.h"
#include "InvertedIndex.h"
#include "SearchEngine.h"

void runRanked(const SearchEngine& engine, const InvertedIndex& index,
               const std::string& query, QueryMode mode, std::size_t k) {
    std::cout << "Query: \"" << query << "\" ["
              << (mode == QueryMode::AND ? "AND" : "OR") << ", top " << k << "]\n";

    std::vector<SearchResult> results = engine.searchRanked(query, mode, k);
    if (results.empty()) {
        std::cout << "  No results.\n";
    }
    for (const SearchResult& r : results) {
        std::cout << "  " << std::fixed << std::setprecision(3) << r.score << "  "
                  << index.getDocument(r.docId)->getName() << "\n";
    }
}

int main(int argc, char* argv[]) {
    std::string directory = (argc > 1) ? argv[1] : "data";

    InvertedIndex index;
    for (const Document& doc : DocumentReader::readDirectory(directory)) {
        index.addDocument(doc);
    }

    SearchEngine engine(index);

    runRanked(engine, index, "index", QueryMode::AND, 3);
    runRanked(engine, index, "words index", QueryMode::OR, 3);
    runRanked(engine, index, "words index", QueryMode::OR, 1);
    runRanked(engine, index, "a", QueryMode::OR, 3);
    runRanked(engine, index, "banana", QueryMode::OR, 3);
    return 0;
}