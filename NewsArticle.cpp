#include "NewsArticle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;
#define MAX_NEWS 25000

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
    if (date.find('-') != string::npos) {
        // Format: YYYY-MM-DD
        return stoi(date.substr(5, 2));
    }
    else if (date.find('/') != string::npos) {
        // Format: YYYY/MM/DD or MM/DD/YYYY
        // If it starts with YYYY, month is at index 5. Otherwise, month is at index 0
        if (isdigit(date[0]) && isdigit(date[1]) && isdigit(date[2]) && isdigit(date[3]) && (date[4] == '/' || date[4] == '-')) {
            // YYYY/MM/DD
            return stoi(date.substr(5, 2));
        }
        else {
            // MM/DD/YYYY
            return stoi(date.substr(0, 2));
        }
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

    // 🔴 Ensure articleCount is reset before loading new dataset
    articleCount = 0;

    // Regex for date validation (YYYY-MM-DD or YYYY/MM/DD)
    std::regex datePattern(R"(^\d{4}[-/]\d{2}[-/]\d{2}$)");

    while (getline(file, line)) {
        if (articleCount >= MAX_NEWS) {
            std::cerr << "Error: Maximum article limit reached (" << MAX_NEWS << ")!\n";
            break;
        }

        // Parse CSV line with manual quote handling
        std::stringstream ss;
        std::string field;
        std::string fields[4];
        int i = 0;
        bool insideQuotes = false;

        for (size_t j = 0; j < line.length(); ++j) {
            if (line[j] == '"') {
                insideQuotes = !insideQuotes;
            }
            // Replace commas inside quotes with special character
            if (insideQuotes && line[j] == ',') {
                ss << '\x01';
            }
            else {
                ss << line[j];
            }
        }

        // Split into 4 fields
        while (getline(ss, field, ',') && i < 4) {
            // Restore commas
            for (char& ch : field) {
                if (ch == '\x01') ch = ',';
            }
            fields[i++] = field;
        }

        if (i < 4) {
            cerr << "Skipping malformed line (too few fields): " << line << endl;
            continue;
        }

        // Extract each field
        std::string title = fields[0];
        std::string text = fields[1];
        std::string subject = fields[2];
        std::string date = fields[3];

        // Trim whitespace
        auto trim = [](std::string& s) {
            const char* ws = " \t\n\r\f\v\"";
            s.erase(0, s.find_first_not_of(ws));
            s.erase(s.find_last_not_of(ws) + 1);
            };
        trim(title);
        trim(text);
        trim(subject);
        trim(date);

        // Remove double quotes inside text
        size_t pos;
        while ((pos = text.find("\"\"")) != std::string::npos) {
            text.replace(pos, 2, "\"");
        }

        // Validate date format
        // We skip lines that do not match the pattern
        if (extractYear(date) == -1) {
            cerr << "Skipping invalid date: " << date << " in line: " << line << endl;
            continue;
        }

        // Store the article
        articles[articleCount++] = { title, text, subject, date };
    }

    file.close();
    cout << "✅ Load complete: " << articleCount << " valid articles loaded from " << filename << endl;
    return articleCount;
}
