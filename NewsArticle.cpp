#include "NewsArticle.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Extracts year from date (Assumes format DD/MM/YYYY)
int extractYear(const std::string& date) {
    if (date.length() >= 10) {
        return stoi(date.substr(6, 4)); // Extract YYYY part
    }
    return 0;
}

// Loads CSV data into the array
int loadCSV(const std::string& filename, NewsArticle* articles, int& articleCount, int maxSize) {
    std::ifstream file(filename);
    std::string line;

    if (!file) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return 0;  // Return 0 articles loaded
    }

    getline(file, line);  // Skip header
    articleCount = 0;  // Reset article count before loading

    while (getline(file, line) && articleCount < maxSize) {
        std::stringstream ss(line);
        std::string title, text, subject, date;

        // Read CSV fields
        if (!getline(ss, title, ',') || !getline(ss, text, ',') ||
            !getline(ss, subject, ',') || !getline(ss, date, ',')) {
            continue;  // Skip if data is incomplete
        }

        // Store in articles array
        articles[articleCount].title = title;
        articles[articleCount].text = text;
        articles[articleCount].subject = subject;
        articles[articleCount].date = date;

        articleCount++;
    }

    file.close();
    std::cout << "Loaded " << articleCount << " articles from " << filename << "!" << std::endl;
    return articleCount;  // Return number of articles loaded
}
