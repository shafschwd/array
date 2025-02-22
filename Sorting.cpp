#include "Sorting.h"
#include "NewsArticle.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>    // For timing
#include <string>

using namespace std;
using namespace std::chrono;

void sortAndDisplaySeparate(NewsArticle articles[], int articleCount) {
    if (articleCount == 0) {
        cout << "Error: No articles loaded.\n";
        return;
    }

    // Use globalCountTrue to determine separation
    int trueCount = globalCountTrue;
    int fakeCount = articleCount - globalCountTrue;

    if (trueCount == 0) {
        cout << "Warning: No true news articles found.\n";
    }
    if (fakeCount == 0) {
        cout << "Warning: No fake news articles found.\n";
    }

    NewsArticle* trueNews = new NewsArticle[trueCount];
    NewsArticle* fakeNews = new NewsArticle[fakeCount];

    // Fill True and Fake article arrays
    for (int i = 0; i < trueCount; i++) {
        trueNews[i] = articles[i];
    }
    for (int i = 0; i < fakeCount; i++) {
        fakeNews[i] = articles[trueCount + i];  // Fake news starts after true news
    }

    // Measure sorting time for True News
    auto startTrue = high_resolution_clock::now();
    mergeSort(trueNews, 0, trueCount - 1);
    auto endTrue = high_resolution_clock::now();
    auto durationTrue = duration_cast<milliseconds>(endTrue - startTrue).count();

    // Measure sorting time for Fake News
    auto startFake = high_resolution_clock::now();
    mergeSort(fakeNews, 0, fakeCount - 1);
    auto endFake = high_resolution_clock::now();
    auto durationFake = duration_cast<milliseconds>(endFake - startFake).count();

    // Display True News
    cout << "\n===============================================================================\n";
    cout << "                      TRUE NEWS ARTICLES (Sorted by Year)          \n";
    cout << "===============================================================================\n";
    cout << left << setw(6) << "No." << "  " << setw(80) << "Title" << "  " << setw(6) << "Year" << "  " << setw(15) << "Category" << endl;
    cout << string(110, '-') << endl;

    for (int i = 0; i < trueCount; i++) {
        string title = trueNews[i].title;
        if (title.length() > 75) {
            title = title.substr(0, 72) + "...";
        }
        cout << left << setw(6) << (i + 1) << "  " << setw(80) << title << "  " << setw(6) << extractYear(trueNews[i].date) << "  " << setw(15) << trueNews[i].subject << endl;
    }
    cout << "===============================================================================\n";
    cout << " Total true news articles: " << trueCount << endl;
    cout << " Sorting completed in " << durationTrue << " ms.\n";
    cout << "===============================================================================\n";

    // Display Fake News
    cout << "\n===============================================================================\n";
    cout << "                      FAKE NEWS ARTICLES (Sorted by Year)          \n";
    cout << "===============================================================================\n";
    cout << left << setw(6) << "No." << "  " << setw(80) << "Title" << "  " << setw(6) << "Year" << "  " << setw(15) << "Category" << endl;
    cout << string(110, '-') << endl;

    for (int i = 0; i < fakeCount; i++) {
        string title = fakeNews[i].title;
        if (title.length() > 75) {
            title = title.substr(0, 72) + "...";
        }
        cout << left << setw(6) << (i + 1) << "  " << setw(80) << title << "  " << setw(6) << extractYear(fakeNews[i].date) << "  " << setw(15) << fakeNews[i].subject << endl;
    }
    cout << "===============================================================================\n";
    cout << " Total fake news articles: " << fakeCount << endl;
    cout << " Sorting completed in " << durationFake << " ms.\n";
    cout << "===============================================================================\n";

    // Cleanup memory
    delete[] trueNews;
    delete[] fakeNews;
}


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


