#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "NewsArticle.h"
#include <string>

struct WordFreq {
    std::string word;
    int count;
};

void countPoliticalFakeNewsMonthly(NewsArticle* arr, int size, int year);
void wordFrequencyGovernment(NewsArticle* arr, int size);

#endif