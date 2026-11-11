#pragma once

#include <string>
#include <vector>

#include "InvertedIndex.h"

enum class QueryMode { AND, OR };

struct SearchResult {
    int docId;
    double score;
};

class SearchEngine {
public:
    explicit SearchEngine(const InvertedIndex& index);

    // Returns IDs of matching documents, sorted ascending.
    std::vector<int> search(const std::string& query, QueryMode mode) const;

    // Returns the top-k documents ranked by TF-IDF, best first.
    std::vector<SearchResult> searchRanked(const std::string& query,
                                           QueryMode mode,
                                           std::size_t k) const;

private:
    const InvertedIndex& index_;
};