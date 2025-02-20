cmake_minimum_required(VERSION 3.30)
project(news)

set(CMAKE_CXX_STANDARD 20)

# Set the directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Add the executable
add_executable(news AssignmentTask1.cpp
        NewsArticle.cpp
        Sorting.cpp
        Searching.cpp
        Analysis.cpp
        NewsArticle.h
        Sorting.h
        Searching.h
        Analysis.h
        Utils.h
)
