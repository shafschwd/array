#include "Analysis.h"
#include <iostream>
#include <sstream>

using namespace std;

int countPoliticalFakeNews(NewsArticle arr[], int size) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i].subject == "politics" && extractYear(arr[i].date) == 2016) {
            count++;
        }
    }
    return count;
}

int countTotalPoliticalNews(NewsArticle arr[], int size) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i].subject == "politics") {
            count++;
        }
    }
    return count;
}

map<string, int> wordFrequencyGovernment(NewsArticle arr[], int size) {
    map<string, int> freq;
    for (int i = 0; i < size; i++) {
        if (arr[i].subject == "government") {
            stringstream ss(arr[i].text);
            string word;
            while (ss >> word) {
                freq[word]++;
            }
        }
    }
    return freq;
}
