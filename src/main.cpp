#include <iostream>
#include <string>
#include <vector>

#include "Document.h"
#include "DocumentReader.h"
#include "Tokenizer.h"

int main(int argc, char* argv[]) {
    std::string directory = (argc > 1) ? argv[1] : "data";

    std::vector<Document> documents = DocumentReader::readDirectory(directory);
    std::cout << "Documents loaded: " << documents.size() << "\n";

    for (const Document& doc : documents) {
        std::vector<std::string> tokens = Tokenizer::tokenize(doc.getContent());
        std::cout << "  [" << doc.getId() << "] " << doc.getName()
                  << " -> " << tokens.size() << " tokens\n";
    }
    return 0;
}