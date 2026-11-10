#pragma once

#include <string>
#include <vector>

#include "Document.h"

class DocumentReader {
public:
    // Reads all .txt files in the directory and returns them as Documents.
    // Returns an empty vector if the directory is invalid.
    static std::vector<Document> readDirectory(const std::string& directoryPath);
};