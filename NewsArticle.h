#ifndef NEWSARTICLE_H
#define NEWSARTICLE_H

#include <string>
#include <vector>

struct NewsArticle {
    std::string title;
    std::string text;
    std::string subject;
    std::string date;
};

// Function declarations
int loadCSV(const std::string& filename, std::vector<NewsArticle>& articles);
int extractYear(const std::string& date);
int extractMonth(const std::string& date);
// void cleanCSV(const std::string& inputFilename, const std::string& outputFilename);

#endif