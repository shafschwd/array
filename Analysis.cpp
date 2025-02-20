#include "Analysis.h"
#include "NewsArticle.h"
#include <iostream>
#include <sstream>
#include "Utils.h" // For toLowerCase(), removePunctuation()

using namespace std;

/**
 * @brief Analyzes and reports the monthly distribution of fake political news for a given year.
 *
 * @param arr   Pointer to the array of NewsArticle
 * @param size  Number of articles in the array
 * @param year  Target year (e.g., 2016)
 */
void countPoliticalFakeNewsMonthly(NewsArticle* arr, int size, int year) {
    int monthlyCount[12] = { 0 };
    int totalFakeNews = 0;

    for (int i = 0; i < size; i++) {
        // Check if subject is "politics" and the year matches
        if (arr[i].subject == "politics" && extractYear(arr[i].date) == year) {
            int month = extractMonth(arr[i].date);
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
    string months[] = { "January", "February", "March", "April", "May", "June",
                        "July", "August", "September", "October", "November", "December" };

    for (int i = 0; i < 12; i++) {
        int percentage = (totalFakeNews == 0)
            ? 0
            : (monthlyCount[i] * 100) / totalFakeNews;
        cout << months[i] << " | ";
        for (int j = 0; j < percentage; j++) {
            cout << "*";
        }
        cout << " " << percentage << "%" << endl;
    }

    cout << "\nNote: Each '*' represents 1% of fake political news articles.\n";
}

const string stopwords[] = {
    "the", "to", "of", "and", "a", "in", "that", "is", "for", "it", "on", "with", "as",
    "was", "at", "by", "an", "be", "this", "which", "or", "from", "but", "are", "not",
    "you", "we", "they", "he", "she", "his", "her", "its", "their", "them", "s", "have", "has",
    "had", "will", "would", "should", "could", "can", "do", "does", "did", "about", "been", "into",
    "i", "t", "who", "us", "all", "our", "your", "my", "me", "him", "am", "were", "there", "where",
    "when", "how", "why", "what", "which", "some", "more", "most", "other", "such", "only", "over", "were",
    "if"
};
const int stopwordCount = sizeof(stopwords) / sizeof(stopwords[0]);

bool isStopword(const string& word) {
    for (int i = 0; i < stopwordCount; i++) {
        if (word == stopwords[i]) return true;
    }
    return false;
}

/**
 * @brief Finds the most frequently used words in fake government-related news.
 *
 * @param arr  Pointer to the array of NewsArticle
 * @param size Number of articles in the array
 */
void wordFrequencyGovernment(NewsArticle* arr, int size) {
    struct WordFreq {
        string word;
        int count = 0;  // Initialize to avoid warnings
    };

    WordFreq wordCounts[37000];
    int wordIndex = 0;

    cout << "Processing words, please wait..." << endl;

    for (int i = 0; i < size; i++) {
        string subject = arr[i].subject;
        toLowerCase(subject);

        if (subject.find("government") != string::npos) {
            stringstream ss(arr[i].text);
            string word;

            while (ss >> word) {
                removePunctuation(word);
                toLowerCase(word);

                if (word.empty() || isStopword(word)) {  // ✅ Skip stopwords
                    continue;
                }

                bool found = false;
                for (int j = 0; j < wordIndex; j++) {
                    if (wordCounts[j].word == word) {
                        wordCounts[j].count++;
                        found = true;
                        break;
                    }
                }

                if (!found && wordIndex < 37000) {
                    wordCounts[wordIndex].word = word;
                    wordCounts[wordIndex].count = 1;
                    wordIndex++;
                }
            }
        }
    }

    if (wordIndex == 0) {
        cout << "No words found in government fake news." << endl;
        return;
    }

    cout << "Total unique words counted: " << wordIndex << endl;

    // Sort words by frequency (manual Bubble Sort)
    for (int i = 0; i < wordIndex - 1; i++) {
        for (int j = i + 1; j < wordIndex; j++) {
            if (wordCounts[j].count > wordCounts[i].count) {
                WordFreq temp = wordCounts[i];
                wordCounts[i] = wordCounts[j];
                wordCounts[j] = temp;
            }
        }
    }

    // Display top 10 words
    cout << "\nMost frequent words in government fake news:\n";
    int limit = (wordIndex < 10) ? wordIndex : 10;
    for (int i = 0; i < limit; i++) {
        cout << wordCounts[i].word << ": " << wordCounts[i].count << endl;
    }
}