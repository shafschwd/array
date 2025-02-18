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

// Function to clean and save the dataset
// void cleanCSV(const std::string& inputFilename, const std::string& outputFilename) {
//     std::ifstream file(inputFilename);
//     std::ofstream cleanFile(outputFilename);
//     std::string line;
//
//     if (!file.is_open()) {
//         std::cerr << "Error: Unable to open file " << inputFilename << std::endl;
//         return;
//     }
//
//     if (!cleanFile.is_open()) {
//         std::cerr << "Error: Unable to create file " << outputFilename << std::endl;
//         return;
//     }
//
//     getline(file, line);  // Read and write header
//     cleanFile << line << "\n";
//
//     std::regex datePattern(R"(^\d{4}[-/]\d{2}[-/]\d{2}$)");  // Matches YYYY-MM-DD or YYYY/MM/DD
//     int validCount = 0, invalidCount = 0;
//
//     while (getline(file, line)) {
//         std::stringstream ss(line);
//         std::vector<std::string> fields;
//         std::string field;
//         bool insideQuotes = false;
//         std::string tempField;
//
//         // Handle quoted fields correctly
//         while (getline(ss, field, ',')) {
//             if (!insideQuotes && field.front() == '"') {
//                 insideQuotes = true;
//                 tempField = field;
//             } else if (insideQuotes && field.back() == '"') {
//                 insideQuotes = false;
//                 tempField += "," + field;
//                 fields.push_back(tempField.substr(1, tempField.length() - 2)); // Remove quotes
//             } else if (insideQuotes) {
//                 tempField += "," + field;
//             } else {
//                 fields.push_back(field);
//             }
//         }
//
//         // Ensure at least 4 fields (Title, Text, Subject, Date)
//         if (fields.size() < 4) {
//             std::cerr << "Skipping malformed line (Too few fields): " << line << std::endl;
//             invalidCount++;
//             continue;
//         }
//
//         // Keep only the first 4 columns in case of extra fields
//         fields.resize(4);
//
//         std::string title = fields[0];
//         std::string text = fields[1];
//         std::string subject = fields[2];
//         std::string date = fields[3];
//
//         // Trim whitespace
//         title.erase(0, title.find_first_not_of(" \""));
//         title.erase(title.find_last_not_of(" \"") + 1);
//         text.erase(0, text.find_first_not_of(" \""));
//         text.erase(text.find_last_not_of(" \"") + 1);
//         subject.erase(0, subject.find_first_not_of(" \""));
//         subject.erase(subject.find_last_not_of(" \"") + 1);
//         date.erase(0, date.find_first_not_of(" \""));
//         date.erase(date.find_last_not_of(" \"") + 1);
//
//         // Ensure the date is an actual date
//         if (!std::regex_match(date, datePattern)) {
//             std::cerr << "Skipping invalid date: " << date << " in line: " << line << std::endl;
//             invalidCount++;
//             continue;
//         }
//
//         // Write valid row to cleaned file
//         cleanFile << "\"" << title << "\",\"" << text << "\",\"" << subject << "\",\"" << date << "\"\n";
//         validCount++;
//     }
//
//     file.close();
//     cleanFile.close();
//
//     std::cout << "✅ Cleaning complete: " << validCount << " valid rows saved, " << invalidCount << " invalid rows removed.\n";
// }


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
        std::vector<std::string> fields;
        std::string field;

        bool insideQuotes = false;
        std::string tempField;

        // Handle quoted fields (to prevent splitting text incorrectly)
        while (getline(ss, field, ',')) {
            if (!insideQuotes && field.front() == '"') {
                insideQuotes = true;
                tempField = field;
            } else if (insideQuotes && field.back() == '"') {
                insideQuotes = false;
                tempField += "," + field;
                fields.push_back(tempField.substr(1, tempField.length() - 2)); // Remove quotes
            } else if (insideQuotes) {
                tempField += "," + field;
            } else {
                fields.push_back(field);
            }
        }

        // Ensure we have exactly 4 fields (Title, Text, Subject, Date)
        if (fields.size() != 4) {
            std::cerr << "Skipping malformed line (Incorrect number of fields): " << line << std::endl;
            continue;
        }

        std::string title = fields[0];
        std::string text = fields[1];
        std::string subject = fields[2];
        std::string date = fields[3];

        // std::cout << "DEBUG: Parsed Date = " << date << std::endl;


        // Trim whitespace
        title.erase(0, title.find_first_not_of(" \""));
        title.erase(title.find_last_not_of(" \"") + 1);
        text.erase(0, text.find_first_not_of(" \""));
        text.erase(text.find_last_not_of(" \"") + 1);
        subject.erase(0, subject.find_first_not_of(" \""));
        subject.erase(subject.find_last_not_of(" \"") + 1);
        date.erase(0, date.find_first_not_of(" \""));
        date.erase(date.find_last_not_of(" \"") + 1);

        // Ensure the date is an actual date (Check if it matches a valid format)
        std::regex datePattern(R"(^\d{4}[-/]\d{2}[-/]\d{2}$)");
        if (!std::regex_match(date, datePattern)) {
            std::cerr << "Skipping invalid date: " << date << " in line: " << line << std::endl;
            continue;
        }

        // Store in articles vector
        articles.push_back({title, text, subject, date});
        articleCount++;
    }

    file.close();
    std::cout << "Loaded " << articleCount << " valid articles from " << filename << "!\n";
    return articleCount;
}