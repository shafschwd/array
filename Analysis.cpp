#include "Analysis.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

void countPoliticalFakeNewsMonthly(NewsArticle* arr, int size, int year) {
    int monthlyCount[12] = {0}; // Stores count for each month
    int totalFakeNews = 0;

    for (int i = 0; i < size; i++) {
        if (arr[i].subject == "politics" && extractYear(arr[i].date) == year) {
            int month = extractMonth(arr[i].date); // Function to extract month from date
            if (month >= 1 && month <= 12) {
                monthlyCount[month - 1]++;
                totalFakeNews++;
            }
        }
    }

    if (totalFakeNews == 0) {
        cout << "No fake political news found for " << year << "." << endl;
        return;
    }

    cout << "\nFake Political News Distribution for " << year << ":\n";
    string months[] = {"January", "February", "March", "April", "May", "June",
                       "July", "August", "September", "October", "November", "December"};

    for (int i = 0; i < 12; i++) {
        int percentage = (monthlyCount[i] * 100) / totalFakeNews;
        cout << months[i] << " | ";
        for (int j = 0; j < percentage; j++) cout << "*";
        cout << " " << percentage << "%" << endl;
    }

    cout << "\nNote: Each '*' represents 1% of fake political news articles." << endl;
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

const string stopwords[] = {
    "the", "to", "of", "and", "a", "in", "that", "is", "for", "it", "on", "with", "as",
    "was", "at", "by", "an", "be", "this", "which", "or", "from", "but", "are", "not",
    "you", "we", "they", "he", "she", "his", "her", "its", "their", "them" , "s", "have", "has",
    "had", "will", "would", "should", "could", "can", "do", "does", "did", "about", "been", "into",
    "i", "t", "who",  "us", "all" , "our", "your", "my", "me", "him", "am", "were", "there", "where",
    "when", "how", "why", "what", "which", "some", "more", "most", "other", "such", "only", "over", "were",
    "if"
};
const int stopwordCount = sizeof(stopwords) / sizeof(stopwords[0]);

bool isStopword(const string& word) {
    for (const auto & stopword : stopwords) {
        if (word == stopword) return true;
    }
    return false;
}


void wordFrequencyGovernment(NewsArticle* arr, int size) {
    WordFreq wordCounts[37000];
    int wordIndex = 0;

    // ✅ Static loading message (No dynamic updates)
    cout << "Processing words, please wait..." << endl;

    for (int i = 0; i < size; i++) {
        string subject = arr[i].subject;
        transform(subject.begin(), subject.end(), subject.begin(), ::tolower);

        if (subject.find("government") != string::npos) {
            stringstream ss(arr[i].text);
            string word;

            while (ss >> word) {
                word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
                transform(word.begin(), word.end(), word.begin(), ::tolower);

                if (word.empty() || isStopword(word)) continue;  // ✅ Ignore stopwords

                bool found = false;
                for (int j = 0; j < wordIndex; j++) {
                    if (wordCounts[j].word == word) {
                        wordCounts[j].count++;
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    if (wordIndex < 37000) {
                        wordCounts[wordIndex].word = word;
                        wordCounts[wordIndex].count = 1;
                        wordIndex++;
                    }
                }
            }
        }
    }

    // ✅ Clear loading message before showing results
    cout << "\r" << string(50, ' ') << "\r"; // Clears the line

    if (wordIndex == 0) {
        cout << "No words found in government fake news." << endl;
        return;
    }

    cout << "Total unique words counted: " << wordIndex << endl;

    for (int i = 0; i < wordIndex - 1; i++) {
        for (int j = i + 1; j < wordIndex; j++) {
            if (wordCounts[j].count > wordCounts[i].count) {
                swap(wordCounts[i], wordCounts[j]);
            }
        }
    }

    cout << "\nMost frequent words in government fake news:\n";
    for (int i = 0; i < min(10, wordIndex); i++) {
        cout << wordCounts[i].word << ": " << wordCounts[i].count << endl;
    }
}



