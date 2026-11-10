#include "SearchEngine.h"

#include <algorithm>
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