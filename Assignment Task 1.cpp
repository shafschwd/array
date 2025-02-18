#include <iostream>
#include "NewsArticle.h"
#include "Sorting.h"
#include "Searching.h"
#include "Analysis.h"

#define MAX_NEWS 25000

using namespace std;

NewsArticle* articles = new NewsArticle[MAX_NEWS];
int articleCount = 0;

// Function to display menu
void menu() {
    int choice;
    do {
        cout << "\n=== News Analysis System ===\n";
        cout << "1. Load Dataset\n";
        cout << "2. Sort Articles by Year\n";
        cout << "3. Count Political Fake News (2016)\n";
        cout << "4. Find Most Frequent Words in Government Fake News\n";
        cout << "5. Search for Articles\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Enter file name (fake.csv or true.csv): ";
            string filename;
            cin >> filename;
            cout << "Loading dataset...\n";
            articleCount = loadCSV(filename, articles, articleCount, MAX_NEWS);
            if (articleCount > 0) {
                cout << "Successfully loaded " << articleCount << " articles from " << filename << "!\n";
            }
            else {
                cout << "Error: Unable to load dataset.\n";
            }
            break;
        }
        case 2:
            if (articleCount == 0) {
                cout << "Error: No articles loaded.\n";
            }
            else {
                cout << "Sorting " << articleCount << " articles...\n";
                measureSortingTime(articles, articleCount);
            }
            break;
        case 3:
            cout << "Fake Political News in 2016: " << countPoliticalFakeNews(articles, articleCount) << endl;
            break;
        case 4:
            wordFrequencyGovernment(articles, articleCount);
            break;
        case 5:
            searchKeyword(articles, articleCount, "Trump");
            break;
        case 6:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice.\n";
            break;
        }
    } while (choice != 6);
}

int main() {
    menu();
    delete[] articles;
    return 0;
}




