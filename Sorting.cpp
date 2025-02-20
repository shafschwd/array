#include "Sorting.h"
#include "NewsArticle.h"
#include <iostream>
#include <iomanip>
#include <ctime>    // For timing
#include <string>

using namespace std;
using namespace std::chrono;

/**
 * @brief Merges two subarrays (for Merge Sort).
 */
void merge(NewsArticle arr[], int left, int mid, int right) {
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    NewsArticle* leftArr = new NewsArticle[leftSize];
    NewsArticle* rightArr = new NewsArticle[rightSize];

    for (int i = 0; i < leftSize; i++) {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < rightSize; j++) {
        rightArr[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < leftSize && j < rightSize) {
        if (extractYear(leftArr[i].date) <= extractYear(rightArr[j].date)) {
            arr[k++] = leftArr[i++];
        }
        else {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < leftSize) {
        arr[k++] = leftArr[i++];
    }
    while (j < rightSize) {
        arr[k++] = rightArr[j++];
    }

    delete[] leftArr;
    delete[] rightArr;
}

/**
 * @brief Merge Sort implementation (recursive).
 */
void mergeSort(NewsArticle arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

/**
 * @brief Bubble Sort for comparing performance with Merge Sort.
 */
void bubbleSort(NewsArticle arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (extractYear(arr[j].date) > extractYear(arr[j + 1].date)) {
                // Swap
                NewsArticle temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

/**
 * @brief Sorts articles by year using Merge Sort and displays them in a table.
 */
void sortArticlesByYear(NewsArticle articles[], int articleCount) {
    if (articleCount == 0) {
        cout << "Error: No articles loaded.\n";
        return;
    }

    cout << "Sorting " << articleCount << " articles with Merge Sort...\n";

    clock_t start = clock();
    mergeSort(articles, 0, articleCount - 1);
    clock_t end = clock();

    double time_taken = double(end - start) / CLOCKS_PER_SEC;

    cout << "Sorting complete in " << fixed << setprecision(3) << time_taken << " seconds.\n";
    cout << "\n==== Sorted Articles by Year (Ascending) ====\n";

    // Column headers
    cout << left << setw(6) << "No." << "  "
        << setw(80) << "Title" << "  "
        << setw(6) << "Year" << "  "
        << setw(15) << "Category" << "\n";
    cout << string(110, '-') << endl;

    for (int i = 0; i < articleCount; i++) {
        string title = articles[i].title;
        // Truncate long titles
        if (title.length() > 75) {
            title = title.substr(0, 72) + "...";
        }
        cout << left << setw(6) << (i + 1) << "  "
            << setw(80) << title << "  "
            << setw(6) << extractYear(articles[i].date) << "  "
            << setw(15) << articles[i].subject << "\n";
    }
}

/**
 * @brief Compares Merge Sort and Bubble Sort performance, prints timing results.
 */
void compareSortingAlgorithms(NewsArticle articles[], int articleCount) {
    if (articleCount == 0) {
        cout << "Error: No articles loaded.\n";
        return;
    }

    // Create copies of the array
    NewsArticle* arrMerge = new NewsArticle[articleCount];
    NewsArticle* arrBubble = new NewsArticle[articleCount];

    for (int i = 0; i < articleCount; i++) {
        arrMerge[i] = articles[i];
        arrBubble[i] = articles[i];
    }

    // MERGE SORT
    clock_t startMerge = clock();
    mergeSort(arrMerge, 0, articleCount - 1);
    clock_t endMerge = clock();
    double mergeTime = double(endMerge - startMerge) / CLOCKS_PER_SEC;

    // BUBBLE SORT
    clock_t startBubble = clock();
    bubbleSort(arrBubble, articleCount);
    clock_t endBubble = clock();
    double bubbleTime = double(endBubble - startBubble) / CLOCKS_PER_SEC;

    cout << "\n=== Sorting Algorithm Comparison ===\n";
    cout << "Merge Sort Time  : " << mergeTime << " seconds\n";
    cout << "Bubble Sort Time : " << bubbleTime << " seconds\n";

    // Cleanup
    delete[] arrMerge;
    delete[] arrBubble;
}

void measureSortingTime(NewsArticle* articles, int articleCount) {
    auto start = std::chrono::high_resolution_clock::now();

    // Call the sorting function
    sortArticlesByYear(articles, articleCount);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Sorting completed in " << duration << " milliseconds." << std::endl;
}


