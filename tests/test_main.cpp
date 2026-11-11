#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "Document.h"
#include "DocumentReader.h"
#include "InvertedIndex.h"
#include "SearchEngine.h"
#include "Tokenizer.h"
#include "Trie.h"

static int g_checks = 0;
static int g_failures = 0;

#define CHECK(cond)                                                          \
    do {                                                                     \
        ++g_checks;                                                          \
        if (!(cond)) {                                                       \
            ++g_failures;                                                    \
            std::cout << "  FAIL line " << __LINE__ << ": " #cond "\n";      \
        }                                                                    \
    } while (0)

#define CHECK_NEAR(a, b) CHECK(std::fabs((a) - (b)) < 1e-6)

using Ids = std::vector<int>;
using Words = std::vector<std::string>;

static void testTokenizer() {
    std::cout << "Tokenizer\n";
    CHECK(Tokenizer::tokenize("").empty());
    CHECK(Tokenizer::tokenize("  ,.;!? ").empty());
    CHECK((Tokenizer::tokenize("Hello, World!") == Words{"hello", "world"}));
    CHECK(Tokenizer::tokenize("go go go").size() == 3);
    CHECK((Tokenizer::tokenize("end") == Words{"end"}));
    CHECK((Tokenizer::tokenize("a1  B2") == Words{"a1", "b2"}));
}

static InvertedIndex buildIndex() {
    InvertedIndex index;
    index.addDocument(Document(0, "d0.txt", "apple banana apple"));
    index.addDocument(Document(1, "d1.txt", "banana cherry"));
    index.addDocument(Document(2, "d2.txt", "cherry date"));
    return index;
}

static void testIndex() {
    std::cout << "InvertedIndex\n";
    InvertedIndex index = buildIndex();
    CHECK(index.documentCount() == 3);
    CHECK(index.vocabularySize() == 4);

    const InvertedIndex::Postings* apple = index.getPostings("apple");
    CHECK(apple != nullptr);
    if (apple != nullptr) {
        CHECK(apple->size() == 1);
        CHECK(apple->at(0) == 2);  // duplicate word counted twice
    }
    CHECK(index.getPostings("missing") == nullptr);
    CHECK(index.getDocument(1) != nullptr);
    CHECK(index.getDocument(99) == nullptr);
    CHECK(index.getDocument(-1) == nullptr);
}

static void testSearch() {
    std::cout << "SearchEngine\n";
    InvertedIndex index = buildIndex();
    SearchEngine engine(index);

    CHECK(engine.search("", QueryMode::OR).empty());
    CHECK(engine.search("   ", QueryMode::AND).empty());
    CHECK(engine.search("missing", QueryMode::OR).empty());
    CHECK((engine.search("apple", QueryMode::OR) == Ids{0}));
    CHECK((engine.search("APPLE!", QueryMode::OR) == Ids{0}));
    CHECK((engine.search("banana cherry", QueryMode::AND) == Ids{1}));
    CHECK((engine.search("banana cherry", QueryMode::OR) == Ids{0, 1, 2}));
    CHECK(engine.search("apple missing", QueryMode::AND).empty());
    CHECK((engine.search("apple missing", QueryMode::OR) == Ids{0}));
}

static void testRanking() {
    std::cout << "Ranking\n";
    InvertedIndex index = buildIndex();
    SearchEngine engine(index);

    CHECK(engine.searchRanked("banana", QueryMode::OR, 0).empty());
    CHECK(engine.searchRanked("", QueryMode::OR, 5).empty());

    std::vector<SearchResult> tie = engine.searchRanked("banana", QueryMode::OR, 5);
    CHECK(tie.size() == 2);
    if (tie.size() == 2) {
        CHECK(tie[0].docId == 0);  // equal scores: lower id first
        CHECK(tie[1].docId == 1);
        CHECK_NEAR(tie[0].score, std::log(1.5));
    }

    std::vector<SearchResult> top = engine.searchRanked("apple banana", QueryMode::OR, 1);
    CHECK(top.size() == 1);
    if (!top.empty()) {
        CHECK(top[0].docId == 0);
        CHECK_NEAR(top[0].score, 2 * std::log(3.0) + std::log(1.5));
    }

    std::vector<SearchResult> many = engine.searchRanked("cherry banana", QueryMode::OR, 10);
    CHECK(many.size() == 3);
    for (std::size_t i = 1; i < many.size(); ++i) {
        CHECK(many[i - 1].score >= many[i].score);  // best first
    }
}

static void testTrie() {
    std::cout << "Trie\n";
    Trie trie;
    for (const std::string& w : {"index", "indexing", "indexes", "tree", "trie"}) {
        trie.insert(w);
    }
    CHECK(trie.contains("index"));
    CHECK(!trie.contains("inde"));
    CHECK(!trie.contains("zzz"));
    CHECK((trie.suggest("ind", 10) == Words{"index", "indexes", "indexing"}));
    CHECK(trie.suggest("ind", 2).size() == 2);
    CHECK(trie.suggest("x", 10).empty());
    CHECK(trie.suggest("ind", 0).empty());
    CHECK(trie.suggest("", 10).size() == 5);
}

static void testReader() {
    std::cout << "DocumentReader\n";
    CHECK(DocumentReader::readDirectory("this_folder_does_not_exist").empty());
}

int main() {
    testTokenizer();
    testIndex();
    testSearch();
    testRanking();
    testTrie();
    testReader();

    std::cout << "\n" << (g_checks - g_failures) << "/" << g_checks << " checks passed\n";
    return g_failures == 0 ? 0 : 1;
}