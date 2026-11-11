#include "SearchEngine.h"

#include <algorithm>
#include <cmath>
#include <queue>
#include <unordered_set>

#include "Tokenizer.h"
SearchEngine::SearchEngine(const InvertedIndex& index) : index_(index) {}

std::vector<int> SearchEngine::search(const std::string& query, QueryMode mode) const {
    std::vector<std::string> words = Tokenizer::tokenize(query);
    if (words.empty()) {
        return {};
    }

    std::unordered_set<int> result;
    bool first = true;

    for (const std::string& word : words) {
        const InvertedIndex::Postings* postings = index_.getPostings(word);

        if (mode == QueryMode::OR) {
            if (postings != nullptr) {
                for (const auto& entry : *postings) {
                    result.insert(entry.first);
                }
            }
        } else {  // AND
            if (postings == nullptr) {
                return {};  // a required word is missing
            }
            if (first) {
                for (const auto& entry : *postings) {
                    result.insert(entry.first);
                }
                first = false;
            } else {
                for (auto it = result.begin(); it != result.end();) {
                    if (postings->count(*it) == 0) {
                        it = result.erase(it);
                    } else {
                        ++it;
                    }
                }
            }
        }
    }

    std::vector<int> ids(result.begin(), result.end());
    std::sort(ids.begin(), ids.end());
    return ids;
}

std::vector<SearchResult> SearchEngine::searchRanked(const std::string& query,
                                                     QueryMode mode,
                                                     std::size_t k) const {
    std::vector<SearchResult> output;
    if (k == 0) {
        return output;
    }

    std::vector<int> candidates = search(query, mode);
    std::vector<std::string> words = Tokenizer::tokenize(query);
    const double totalDocs = static_cast<double>(index_.documentCount());

    // Returns true if a is BETTER than b (higher score, then lower id).
    auto better = [](const SearchResult& a, const SearchResult& b) {
        if (a.score != b.score) return a.score > b.score;
        return a.docId < b.docId;
    };

    // priority_queue keeps the element that compares "largest" on top.
    // Using `better` as the comparator puts the WORST result on top (min-heap).
    std::priority_queue<SearchResult, std::vector<SearchResult>, decltype(better)>
        heap(better);

    for (int docId : candidates) {
        double score = 0.0;
        for (const std::string& word : words) {
            const InvertedIndex::Postings* postings = index_.getPostings(word);
            if (postings == nullptr) {
                continue;
            }
            auto it = postings->find(docId);
            if (it == postings->end()) {
                continue;
            }
            double tf = static_cast<double>(it->second);
            double df = static_cast<double>(postings->size());
            score += tf * std::log(totalDocs / df);
        }

        heap.push({docId, score});
        if (heap.size() > k) {
            heap.pop();  // drop the worst result
        }
    }

    while (!heap.empty()) {
        output.push_back(heap.top());
        heap.pop();
    }
    std::reverse(output.begin(), output.end());  // worst-first -> best-first
    return output;
}