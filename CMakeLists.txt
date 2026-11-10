cmake_minimum_required(VERSION 3.16)
project(Seekr VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json so VS Code IntelliSense works
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(seekr
    src/main.cpp
    src/Document.cpp
    src/Tokenizer.cpp
    src/DocumentReader.cpp
    src/InvertedIndex.cpp
    src/SearchEngine.cpp
)
target_include_directories(seekr PRIVATE include)

target_compile_options(seekr PRIVATE -Wall -Wextra -Wpedantic)