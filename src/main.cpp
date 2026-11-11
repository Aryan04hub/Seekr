#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "DocumentReader.h"
#include "InvertedIndex.h"
#include "SearchEngine.h"
#include "Trie.h"

namespace {

const std::size_t kTopK = 5;
const std::size_t kSuggestLimit = 5;

void printHelp() {
    std::cout << "Commands:\n"
              << "  search <words>   rank documents for the query\n"
              << "  suggest <prefix> autocomplete a word prefix\n"
              << "  mode and|or      switch between AND and OR queries\n"
              << "  help             show this message\n"
              << "  quit             exit\n";
}

void doSearch(const SearchEngine& engine, const InvertedIndex& index,
              const std::string& query, QueryMode mode) {
    std::vector<SearchResult> results = engine.searchRanked(query, mode, kTopK);
    if (results.empty()) {
        std::cout << "  No results.\n";
        return;
    }
    for (const SearchResult& r : results) {
        std::cout << "  " << std::fixed << std::setprecision(3) << r.score << "  "
                  << index.getDocument(r.docId)->getName() << "\n";
    }
}

void doSuggest(const Trie& trie, const std::string& prefix) {
    std::vector<std::string> words = trie.suggest(prefix, kSuggestLimit);
    if (words.empty()) {
        std::cout << "  No suggestions.\n";
        return;
    }
    for (const std::string& w : words) {
        std::cout << "  " << w << "\n";
    }
}

}  // namespace

int main(int argc, char* argv[]) {
    std::string directory = (argc > 1) ? argv[1] : "data";

    InvertedIndex index;
    for (const Document& doc : DocumentReader::readDirectory(directory)) {
        index.addDocument(doc);
    }

    Trie trie;
    for (const std::string& word : index.vocabulary()) {
        trie.insert(word);
    }

    SearchEngine engine(index);
    QueryMode mode = QueryMode::OR;

    std::cout << "Seekr - Mini Search Engine\n"
              << "Indexed " << index.documentCount() << " documents, "
              << index.vocabularySize() << " unique words.\n"
              << "Type 'help' for commands.\n";

    std::string line;
    while (true) {
        std::cout << "\nseekr> ";
        if (!std::getline(std::cin, line)) {
            break;  // end of input (Ctrl+D)
        }

        std::istringstream stream(line);
        std::string command;
        stream >> command;
        std::string rest;
        std::getline(stream >> std::ws, rest);

        if (command.empty()) {
            continue;
        } else if (command == "quit" || command == "exit") {
            break;
        } else if (command == "help") {
            printHelp();
        } else if (command == "mode") {
            if (rest == "and") {
                mode = QueryMode::AND;
                std::cout << "  Mode: AND\n";
            } else if (rest == "or") {
                mode = QueryMode::OR;
                std::cout << "  Mode: OR\n";
            } else {
                std::cout << "  Usage: mode and|or\n";
            }
        } else if (command == "search") {
            doSearch(engine, index, rest, mode);
        } else if (command == "suggest") {
            doSuggest(trie, rest);
        } else {
            std::cout << "  Unknown command. Type 'help'.\n";
        }
    }

    std::cout << "Goodbye.\n";
    return 0;
}