#include "DocumentReader.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <system_error>
#include <utility>

namespace fs = std::filesystem;

namespace {

bool readFile(const fs::path& path, std::string& out) {
    std::ifstream in(path);
    if (!in) {
        return false;
    }
    std::ostringstream buffer;
    buffer << in.rdbuf();
    out = buffer.str();
    return true;
}

}  // namespace

std::vector<Document> DocumentReader::readDirectory(const std::string& directoryPath) {
    std::vector<Document> documents;
    std::error_code ec;

    if (!fs::is_directory(directoryPath, ec)) {
        std::cerr << "Error: '" << directoryPath << "' is not a valid directory.\n";
        return documents;
    }

    std::vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(directoryPath, ec)) {
        std::error_code entryEc;
        if (entry.is_regular_file(entryEc) && entry.path().extension() == ".txt") {
            files.push_back(entry.path());
        }
    }

    std::sort(files.begin(), files.end());

    int nextId = 0;
    for (const fs::path& path : files) {
        std::string content;
        if (!readFile(path, content)) {
            std::cerr << "Warning: could not read '" << path.string() << "', skipping.\n";
            continue;
        }
        documents.emplace_back(nextId++, path.filename().string(), std::move(content));
    }

    return documents;
}