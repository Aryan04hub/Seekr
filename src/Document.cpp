#include "Document.h"

#include <utility>

Document::Document(int id, std::string name, std::string content)
    : id_(id), name_(std::move(name)), content_(std::move(content)) {}

int Document::getId() const { return id_; }

const std::string& Document::getName() const { return name_; }

const std::string& Document::getContent() const { return content_; }