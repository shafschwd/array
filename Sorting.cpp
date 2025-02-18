#include "Sorting.h"
#include <iostream>
#include <chrono>

using namespace std;

// Partition function for QuickSort
int partition(NewsArticle arr[], int low, int high) {
    if (low < 0 || high < 0 || low >= high) {
        cout << "Error: Invalid partition bounds! Low=" << low << ", High=" << high << endl;
        return low; // Prevent crash
    }

    int pivotYear = extractYear(arr[high].date);
    if (pivotYear == 0) { // If invalid year, prevent crash
        cout << "Error: Invalid pivot year detected! Skipping...\n";
        return low;
    }

    int i = low - 1;
    for (int j = low; j < high; j++) {
        int currentYear = extractYear(arr[j].date);
        if (currentYear == 0) continue; // Skip invalid dates

        if (currentYear < pivotYear) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// QuickSort implementation
void quickSort(NewsArticle arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        if (pi == low || pi == high) {  // Prevent infinite recursion
            return;
        }
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Measure sorting performance
void measureSortingTime(NewsArticle arr[], int size) {
    if (size <= 0) {
        cout << "Error: No data available for sorting.\n";
        return;
    }

    auto start = chrono::high_resolution_clock::now();
    quickSort(arr, 0, size - 1);
    auto end = chrono::high_resolution_clock::now();

    cout << "Sorting Time: " << chrono::duration<double>(end - start).count() << " seconds\n";
}


