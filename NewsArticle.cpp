#include "NewsArticle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <stdexcept>

using namespace std;
#define MAX_NEWS 25000

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

int extractMonth(const string& date) {
    if (date.find('-') != string::npos) {
        // Format: YYYY-MM-DD
        return stoi(date.substr(5, 2));
    } else if (date.find('/') != string::npos) {
        // Format: MM/DD/YYYY
        return stoi(date.substr(0, 2));
    }
    return -1; // Return -1 for invalid format
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


// Improved loadCSV function that correctly handles nested quotes and malformed rows
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include "NewsArticle.h"

using namespace std;

int loadCSV(const std::string& filename, NewsArticle articles[], int& articleCount) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return 0;
    }

    getline(file, line);  // Skip header
    articleCount = 0;  // Reset count

    std::regex datePattern(R"(^\d{4}[-/]\d{2}[-/]\d{2}$)");  // Matches YYYY-MM-DD or YYYY/MM/DD

    while (getline(file, line)) {
        if (articleCount >= MAX_NEWS) {
            std::cerr << "Error: Maximum article limit reached!" << std::endl;
            break;
        }

        std::stringstream ss;
        std::string field;
        std::string fields[4];  // Fixed-size array for 4 fields
        int i = 0;
        bool insideQuotes = false;
        std::string tempField;

        // Fix incorrect splitting due to commas inside quoted text
        for (size_t j = 0; j < line.length(); ++j) {
            if (line[j] == '"') {
                insideQuotes = !insideQuotes;
            }
            if (insideQuotes && line[j] == ',') {
                ss << '\x01';  // Replace commas inside quotes with a special character
            } else {
                ss << line[j];
            }
        }

        // Read fields and restore commas inside quoted text
        while (getline(ss, field, ',') && i < 4) {
            for (char &ch : field) {
                if (ch == '\x01') ch = ',';  // Restore commas inside quoted text
            }
            fields[i++] = field;
        }

        // Ensure exactly 4 fields (Title, Text, Subject, Date)
        if (i < 4) {
            std::cerr << "Skipping malformed line (Too few fields): " << line << std::endl;
            continue;
        }

        std::string title = fields[0];
        std::string text = fields[1];
        std::string subject = fields[2];
        std::string date = fields[3];

        // Trim whitespace
        title.erase(0, title.find_first_not_of(" \""));
        title.erase(title.find_last_not_of(" \"") + 1);
        text.erase(0, text.find_first_not_of(" \""));
        text.erase(text.find_last_not_of(" \"") + 1);
        subject.erase(0, subject.find_first_not_of(" \""));
        subject.erase(subject.find_last_not_of(" \"") + 1);
        date.erase(0, date.find_first_not_of(" \""));
        date.erase(date.find_last_not_of(" \"") + 1);

        // Fix double quotes inside text fields (e.g., `""word""` → `"word"`)
        size_t pos;
        while ((pos = text.find("\"\"")) != std::string::npos) {
            text.replace(pos, 2, "\"");
        }

        // Validate date format
        if (!std::regex_match(date, datePattern)) {
            std::cerr << "Skipping invalid date: " << date << " in line: " << line << std::endl;
            continue;
        }

        // Store valid article
        articles[articleCount++] = {title, text, subject, date};
    }

    file.close();

    std::cout << "✅ Load complete: " << articleCount << " valid articles loaded.\n";
    return articleCount;
}


//end of NewsArticle.cpp