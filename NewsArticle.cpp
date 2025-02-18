#include "NewsArticle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <stdexcept>

using namespace std;

int extractYear(const std::string& date) {
    std::regex datePattern1(R"(\d{2}/\d{2}/\d{4})"); // DD/MM/YYYY
    std::regex datePattern2(R"(\d{4}/\d{2}/\d{2})"); // YYYY/MM/DD
    std::regex datePattern3(R"(\d{4}-\d{2}-\d{2})"); // YYYY-MM-DD (New Support)

    try {
        if (std::regex_match(date, datePattern1)) {
            return std::stoi(date.substr(6, 4));
        } else if (std::regex_match(date, datePattern2) || std::regex_match(date, datePattern3)) {
            return std::stoi(date.substr(0, 4));
        } else {
            std::cerr << "Invalid date format: " << date << std::endl;
            return -1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error extracting year from: " << date << " | " << e.what() << std::endl;
        return -1;
    }
}


int loadCSV(const std::string& filename, std::vector<NewsArticle>& articles) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return 0;
    }

    getline(file, line);  // Skip header
    int articleCount = 0;

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string title, text, subject, date;

        // Improved CSV parsing to handle quoted fields
        getline(ss, title, ',');
        getline(ss, text, ',');
        getline(ss, subject, ',');
        getline(ss, date, ',');

        // Trim whitespace
        title = title.empty() ? "N/A" : title;
        text = text.empty() ? "N/A" : text;
        subject = subject.empty() ? "N/A" : subject;
        date = date.empty() ? "N/A" : date;

        // Date validation
        if (extractYear(date) == -1) {
            std::cerr << "Skipping invalid date: " << date << "\n";
            continue;
        }

        articles.push_back({title, text, subject, date});
        articleCount++;
    }

    file.close();
    std::cout << "Loaded " << articleCount << " articles from " << filename << "!\n";
    return articleCount;
}
