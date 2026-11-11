#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "Document.h"
#include "InvertedIndex.h"
#include "SearchEngine.h"
#include "Trie.h"

using Clock = std::chrono::steady_clock;

static double millis(Clock::time_point a, Clock::time_point b) {
    return std::chrono::duration<double, std::milli>(b - a).count();
}

static std::string makeText(std::mt19937& rng, const std::vector<std::string>& vocab,
                            int words) {
    std::uniform_int_distribution<std::size_t> pick(0, vocab.size() - 1);
    std::string text;
    for (int i = 0; i < words; ++i) {
        text += vocab[pick(rng)];
        text += ' ';
    }
    return text;
}

int main() {
    std::vector<std::string> vocab;
    for (int i = 0; i < 5000; ++i) {
        vocab.push_back("word" + std::to_string(i));
    }

    const int wordsPerDoc = 200;
    const int repeats = 1000;
    const std::vector<int> sizes = {100, 1000, 10000};

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Docs    Index(ms)  Trie(ms)  SearchOR(us)  SearchAND(us)\n";

    for (int n : sizes) {
        std::mt19937 rng(42);
        std::vector<Document> docs;
        docs.reserve(static_cast<std::size_t>(n));
        for (int i = 0; i < n; ++i) {
            docs.emplace_back(i, "doc" + std::to_string(i), makeText(rng, vocab, wordsPerDoc));
        }

        InvertedIndex index;
        auto t0 = Clock::now();
        for (const Document& d : docs) {
            index.addDocument(d);
        }
        auto t1 = Clock::now();

        Trie trie;
        for (const std::string& w : index.vocabulary()) {
            trie.insert(w);
        }
        auto t2 = Clock::now();

        SearchEngine engine(index);
        volatile std::size_t sink = 0;

        auto t3 = Clock::now();
        for (int r = 0; r < repeats; ++r) {
            sink = sink + engine.searchRanked("word10 word20 word30", QueryMode::OR, 5).size();
        }
        auto t4 = Clock::now();
        for (int r = 0; r < repeats; ++r) {
            sink = sink + engine.searchRanked("word10 word20 word30", QueryMode::AND, 5).size();
        }
        auto t5 = Clock::now();

        std::cout << std::setw(6) << n
                  << std::setw(12) << millis(t0, t1)
                  << std::setw(10) << millis(t1, t2)
                  << std::setw(14) << millis(t3, t4) * 1000.0 / repeats
                  << std::setw(15) << millis(t4, t5) * 1000.0 / repeats << "\n";
    }
    return 0;
}