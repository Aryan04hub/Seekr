#pragma once

#include <string>

class Document {
public:
    Document(int id, std::string name, std::string content);

    int getId() const;
    const std::string& getName() const;
    const std::string& getContent() const;

private:
    int id_;
    std::string name_;
    std::string content_;
};