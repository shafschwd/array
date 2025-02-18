#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "NewsArticle.h"
#include <map>

int countPoliticalFakeNews(NewsArticle arr[], int size);
int countTotalPoliticalNews(NewsArticle arr[], int size);
std::map<std::string, int> wordFrequencyGovernment(NewsArticle arr[], int size);

#endif


