#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "NewsArticle.h"

/**
 * @brief Analyzes and reports the monthly distribution of fake political news for a given year.
 *
 * @param arr   Pointer to the array of NewsArticle
 * @param size  Number of articles in the array
 * @param year  Target year
 */
void countPoliticalFakeNewsMonthly(NewsArticle* arr, int size, int year);

/**
 * @brief Finds the most frequently used words in fake government-related news.
 *
 * @param arr  Pointer to the array of NewsArticle
 * @param size Number of articles in the array
 */
void wordFrequencyGovernment(NewsArticle* arr, int size);

#endif
