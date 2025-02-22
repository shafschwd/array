#ifndef SEARCHING_H
#define SEARCHING_H

#include "NewsArticle.h"
#include <string>

/**
 * @brief Searches for articles containing a given keyword in the title or text.
 *
 * @param arr       Array of NewsArticle
 * @param size      Number of articles
 * @param keyword   Keyword to search for
 */
void searchKeyword(NewsArticle arr[], int size, const std::string& keyword);
void getAvailableCategories(NewsArticle articles[], int articleCount, int selectedYear, std::string availableCategories[], int& categoryCount);

#endif

