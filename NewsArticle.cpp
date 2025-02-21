#include "NewsArticle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;
#define MAX_NEWS 50000

/**
 * @brief Extracts the year from a date string in multiple formats.
 *
 * @param date The date string (DD/MM/YYYY or YYYY/MM/DD or YYYY-MM-DD)
 * @return int  The year as an integer, or -1 if invalid
 */
int extractYear(const std::string& date) {
    // Supported formats
    std::regex datePattern1(R"(\d{2}/\d{2}/\d{4})"); // DD/MM/YYYY
    std::regex datePattern2(R"(\d{4}/\d{2}/\d{2})"); // YYYY/MM/DD
    std::regex datePattern3(R"(\d{4}-\d{2}-\d{2})"); // YYYY-MM-DD

    try {
        if (std::regex_match(date, datePattern1)) {
            // Example: 12/31/2016
            return std::stoi(date.substr(6, 4));
        }
        else if (std::regex_match(date, datePattern2) || std::regex_match(date, datePattern3)) {
            // Example: 2016/12/31 or 2016-12-31
            return std::stoi(date.substr(0, 4));
        }
        else {
            std::cerr << "Invalid date format: " << date << std::endl;
            return -1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error extracting year from: " << date << " | " << e.what() << std::endl;
        return -1;
    }
}

/**
 * @brief Extracts the month from a date string in multiple formats.
 *
 * @param date The date string (DD/MM/YYYY or YYYY/MM/DD or YYYY-MM-DD)
 * @return int  The month as an integer, or -1 if invalid
 */
int extractMonth(const string& date) {
    if (date.length() < 7) return -1;  // Ensure valid format

    try {
        if (date.find('-') != string::npos) {
            return stoi(date.substr(5, 2)); // Format: YYYY-MM-DD
        } else if (date.find('/') != string::npos) {
            return stoi(date.substr(5, 2)); // Format: MM/DD/YYYY
        }
    } catch (const std::exception& e) {
        std::cerr << "Error extracting month from: " << date << " | " << e.what() << std::endl;
    }

    return -1; // Return -1 for invalid format
}
/**
 * @brief Loads CSV data into an array of NewsArticle objects.
 *
 * @param filename      Path to the CSV file
 * @param articles      Array of NewsArticle to store the data
 * @param articleCount  Current count of articles in the array
 * @return int          Updated article count
 */
int loadCSV(const std::string& filename, NewsArticle articles[], int& articleCount) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return 0;
    }

    std::string line;
    getline(file, line);  // Skip header if present

    int initialCount = articleCount;  // Store initial count
    int count = 0;  // Local count for this file

    while (getline(file, line)) {
        if (articleCount >= MAX_NEWS) {
            std::cerr << "Error: Maximum article limit reached (" << MAX_NEWS << ")!\n";
            break;
        }

        // Parse fields manually
        std::string fields[4];
        int i = 0;
        std::string temp;
        bool insideQuotes = false;

        for (char ch : line) {
            if (ch == '"') insideQuotes = !insideQuotes;
            if (!insideQuotes && ch == ',') {
                fields[i++] = temp;
                temp.clear();
            } else {
                temp += ch;
            }
        }
        if (i == 3) fields[3] = temp;  // Last field (Date)

        // Ensure valid fields
        if (i < 3) {
            std::cerr << "Skipping malformed line (too few fields): " << line << std::endl;
            continue;
        }

        // Trim and store the article
        auto trim = [](std::string& s) {
            const char* ws = " \t\n\r\f\v\"";
            s.erase(0, s.find_first_not_of(ws));
            s.erase(s.find_last_not_of(ws) + 1);
        };

        for (int j = 0; j < 4; j++) {
            trim(fields[j]);
        }

        // Validate Date
        if (extractYear(fields[3]) == -1) {
            std::cerr << "Skipping invalid date: " << fields[3] << " in line: " << line << std::endl;
            continue;
        }

        // Store in array
        articles[articleCount].title = fields[0];
        articles[articleCount].text = fields[1];
        articles[articleCount].subject = fields[2];
        articles[articleCount].date = fields[3];

        articleCount++;
        count++;
    }

    file.close();
    return count;
}



