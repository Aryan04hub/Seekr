#include <iostream>

#include "Document.h"

int main() {
    Document doc(1, "sample.txt", "Hello Seekr search engine");

    std::cout << "ID: " << doc.getId() << "\n";
    std::cout << "Name: " << doc.getName() << "\n";
    std::cout << "Content: " << doc.getContent() << "\n";
    return 0;
}