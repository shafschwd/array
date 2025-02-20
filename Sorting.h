#ifndef SORTING_H
#define SORTING_H

#include "NewsArticle.h"

/**
 * @brief Recursively performs Merge Sort on an array of NewsArticle.
 */
void mergeSort(NewsArticle arr[], int left, int right);

/**
 * @brief Performs Bubble Sort on an array of NewsArticle.
 */
void bubbleSort(NewsArticle arr[], int n);

/**
 * @brief Sorts articles by year (ascending) using Merge Sort and displays them.
 */
void sortArticlesByYear(NewsArticle articles[], int articleCount);

/**
 * @brief Compares Merge Sort vs Bubble Sort for performance.
 */
void compareSortingAlgorithms(NewsArticle articles[], int articleCount);

#endif

