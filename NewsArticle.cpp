#include "NewsArticle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <regex>

using namespace std;


int extractYear(const std::string& date) {
    // Regular expressions to match different date formats
    std::regex datePattern1(R"(\d{2}/\d{2}/\d{4})"); // DD/MM/YYYY
    std::regex datePattern2(R"(\d{4}-\d{2}-\d{2})"); // YYYY-MM-DD

    if (std::regex_match(date, datePattern1)) {
        std::string yearStr = date.substr(6, 4);
        try {
            return std::stoi(yearStr);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << " for date: " << date << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Out of range: " << e.what() << " for date: " << date << std::endl;
        }
    } else if (std::regex_match(date, datePattern2)) {
        std::string yearStr = date.substr(0, 4);
        try {
            return std::stoi(yearStr);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << " for date: " << date << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Out of range: " << e.what() << " for date: " << date << std::endl;
        }
    } else {
        std::cerr << "Invalid date format: " << date << std::endl;
    }
    return 0; // Return 0 if conversion fails
}

// Loads CSV data into the array with additional validation checks
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

        // Additional validation checks
        if (title.empty() || text.empty() || subject.empty() || extractYear(date) == 0) {
            std::cerr << "Invalid data: " << line << std::endl;
            continue;  // Skip invalid data
        }

        // Store in articles array
        articles[articleCount].title = title;
        articles[articleCount].text = text;
        articles[articleCount].subject = subject;
        articles[articleCount].date = date;

        articleCount++;
    }

    file.close();

    if (articleCount == 0) {
        std::cerr << "No data in file " << filename << std::endl;
    } else {
        std::cout << "Loaded " << articleCount << " articles from " << filename << "!" << std::endl;
    }

    return articleCount;  // Return number of articles loaded
}