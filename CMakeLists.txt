cmake_minimum_required(VERSION 3.16)
project(Seekr VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(seekr_core STATIC
    src/Document.cpp
    src/Tokenizer.cpp
    src/DocumentReader.cpp
    src/InvertedIndex.cpp
    src/SearchEngine.cpp
    src/Trie.cpp
)
target_include_directories(seekr_core PUBLIC include)
target_compile_options(seekr_core PRIVATE -Wall -Wextra -Wpedantic)

add_executable(seekr src/main.cpp)
target_link_libraries(seekr PRIVATE seekr_core)
target_compile_options(seekr PRIVATE -Wall -Wextra -Wpedantic)

add_executable(seekr_tests tests/test_main.cpp)
target_link_libraries(seekr_tests PRIVATE seekr_core)
target_compile_options(seekr_tests PRIVATE -Wall -Wextra -Wpedantic)

enable_testing()
add_test(NAME seekr_tests COMMAND seekr_tests)