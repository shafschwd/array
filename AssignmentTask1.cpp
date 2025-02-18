#include <iostream>
#include <filesystem>
#include "NewsArticle.h"
#include "Sorting.h"
#include "Searching.h"
#include "Analysis.h"

#define MAX_NEWS 25000

using namespace std;
namespace fs = std::filesystem;

NewsArticle* articles = new NewsArticle[MAX_NEWS];
int articleCount = 0;

// Function to display menu
void menu() {
    int choice;
    do {
        cout << "\n=== News Analysis System ===\n";
        cout << "1. Sort Articles by Year\n";
        cout << "2. Count Political Fake News (2016)\n";
        cout << "3. Find Most Frequent Words in Government Fake News\n";
        cout << "4. Search for Articles\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (articleCount == 0) {
                    cout << "Error: No articles loaded.\n";
                } else {
                    cout << "Sorting " << articleCount << " articles...\n";
                    measureSortingTime(articles, articleCount);
                }
            break;
            case 2:
                cout << "Fake Political News in 2016: " << countPoliticalFakeNews(articles, articleCount) << endl;
            break;
            case 3:
                wordFrequencyGovernment(articles, articleCount);
            break;
            case 4:
                searchKeyword(articles, articleCount, "Trump");
            break;
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

// Function to filter out bad data
void filterBadData(NewsArticle* articles, int& articleCount) {
    int validCount = 0;
    for (int i = 0; i < articleCount; ++i) {
        // Check if all fields are non-empty and the date is valid
        if (!articles[i].title.empty() && !articles[i].text.empty() &&
            !articles[i].subject.empty() && extractYear(articles[i].date) != 0) {
            articles[validCount++] = articles[i];
            } else {
                std::cerr << "Invalid data: " << articles[i].title << " | "
                          << articles[i].text << " | " << articles[i].subject << " | "
                          << articles[i].date << std::endl;
            }
    }
    articleCount = validCount;
    cout << "Filtered dataset, remaining " << articleCount << " valid articles.\n";
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
            articleCount = loadCSV(selectedDataset, articles, articleCount, MAX_NEWS);
            if (articleCount > 0) {
                cout << "Successfully loaded " << articleCount << " articles from " << selectedDataset << "!\n";
                filterBadData(articles, articleCount); // Filter bad data after loading
                break; // Exit the loop if data is successfully loaded
            } else {
                cout << "No data in file. Please select another dataset.\n";
            }
        } else {
            cout << "Invalid dataset choice. Please select a valid dataset.\n";
        }
    }

    menu();
    delete[] articles;
    return 0;
}