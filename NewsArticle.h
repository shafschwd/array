#ifndef NEWSARTICLE_H
#define NEWSARTICLE_H

#include <string>

/**
 * @brief A structure representing a news article.
 */
struct NewsArticle {
    std::string title;
    std::string text;
    std::string subject;
    std::string date;
};

// Function declarations
int loadCSV(const std::string& filename, NewsArticle articles[], int& articleCount);
int extractYear(const std::string& date);
int extractMonth(const std::string& date);

#endif
