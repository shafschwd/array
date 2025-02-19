#include <iostream>
#include <filesystem>
#include "NewsArticle.h"
#include "Sorting.h"
#include "Searching.h"
#include "Analysis.h"

#define MAX_NEWS 25000

using namespace std;
namespace fs = std::filesystem;

NewsArticle articles[MAX_NEWS];  // Fixed-size array
int articleCount = 0;

// Function to display menu
void menu() {
    int choice;
    do {
        cout << "\n=== News Analysis System ===\n";
        cout << "1. Sort Articles by Year\n";
        cout << "2. Count Political Fake News\n";
        cout << "3. Find Most Frequent Words in Government Fake News\n";
        cout << "4. Search for Articles\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";

        while (!(cin >> choice)) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter an integer: ";
        }

        switch (choice) {
            case 1:
                if (articleCount == 0) {
                    cout << "Error: No articles loaded.\n";
                } else {
                    cout << "Sorting " << articleCount << " articles...\n";
                    measureSortingTime(articles, articleCount);
                }
                break;
            case 2: {
                int yearChoice;
                cout << "Choose the year:\n";
                cout << "1. 2015\n";
                cout << "2. 2016\n";
                cout << "3. 2017\n";
                cout << "Enter choice: ";
                while (!(cin >> yearChoice) || yearChoice < 1 || yearChoice > 3) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter 1, 2, or 3: ";
                }
                int year = 2015 + (yearChoice - 1);
                countPoliticalFakeNewsMonthly(articles, articleCount, year);
                break;
            }
            case 3:
                wordFrequencyGovernment(articles, articleCount);
                break;
            case 4: {
                string keyword;
                cout << "Enter keyword to search for: ";
                cin.ignore();
                getline(cin, keyword);
                searchKeyword(articles, articleCount, keyword);
                break;
            }
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice.\n";
                break;
        }
    } while (choice != 5);
}

// Function to list datasets in the Dataset folder
void listDatasets() {
    cout << "Available datasets:\n";
    int index = 1;
    for (const auto& entry : fs::directory_iterator("Dataset")) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            cout << index++ << ". " << entry.path().filename().string() << endl;
        }
    }
}

int main() {
    cout << "Checking for datasets in the 'Dataset' folder...\n";
    listDatasets();

    while (true) {
        cout << "Enter the number of the dataset you want to load: ";
        int datasetChoice;
        while (!(cin >> datasetChoice)) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter an integer: ";
        }

        int index = 1;
        string selectedDataset;
        for (const auto& entry : fs::directory_iterator("Dataset")) {
            if (entry.is_regular_file() && entry.path().extension() == ".csv" && index++ == datasetChoice) {
                selectedDataset = entry.path().string();
                break;
            }
        }

        if (!selectedDataset.empty()) {
            cout << "Loading dataset " << selectedDataset << "...\n";
            articleCount = loadCSV(selectedDataset, articles, articleCount);
            if (articleCount == 0) {
                std::cerr << "No valid data found in file " << selectedDataset << std::endl;
            } else {
                std::cout << "Successfully loaded " << articleCount << " articles from " << selectedDataset << "!\n";
            }
            if (articleCount > 0) {
                cout << "Successfully loaded " << articleCount << " articles from " << selectedDataset << "!\n";
                break; // Exit the loop if data is successfully loaded
            } else {
                cout << "No data in file. Please select another dataset.\n";
            }
        } else {
            cout << "Invalid dataset choice. Please select a valid dataset.\n";
        }
    }

    menu();
    return 0;
}

