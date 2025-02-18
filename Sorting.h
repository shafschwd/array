#ifndef SORTING_H
#define SORTING_H

#include "NewsArticle.h"

void quickSort(NewsArticle arr[], int low, int high);
void measureSortingTime(NewsArticle arr[], int size);
int partition(NewsArticle arr[], int low, int high);

#endif

