#pragma once

#include <string>
#include <vector>

#include "InvertedIndex.h"

enum class QueryMode { AND, OR };

class SearchEngine {
public:
    explicit SearchEngine(const InvertedIndex& index);

    // Returns IDs of matching documents, sorted ascending.
    std::vector<int> search(const std::string& query, QueryMode mode) const;

private:
    const InvertedIndex& index_;
};