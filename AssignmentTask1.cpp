#include <iostream>
#include <filesystem>
#include <limits>
#include <ctime>  
#include <iomanip>  // ✅ For proper table formatting
#include "NewsArticle.h"
#include "Sorting.h"
#include "Searching.h"
#include "Analysis.h"

#define MAX_NEWS 25000

using namespace std;
namespace fs = std::filesystem;

// Global array for storing NewsArticles
NewsArticle articles[MAX_NEWS];
int articleCount = 0;

// List of predefined categories
const string categories[] = { "politics", "business", "entertainment", "health", "science", "sports", "technology" };
const int categoryCount = sizeof(categories) / sizeof(categories[0]);

void displayCategories() {
    cout << "\nAvailable Categories:\n";
    for (int i = 0; i < categoryCount; i++) {
        cout << " " << (i + 1) << ". " << categories[i] << "\n";
    }
}

void menu() {
    while (true) {
        cout << "\n========== MENU ==========\n"
            << "1. Display Total Articles\n"
            << "2. Display First 5 Articles\n"
            << "3. Sort Articles by Year\n"
            << "4. Search Articles by Year and Category\n"
            << "5. Analyze Fake Political News by Year\n"
            << "6. Analyze Most Frequent Words in Fake News Article\n"
            << "7. Exit\n"
            << "===========================\n"
            << "Enter your choice: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Invalid input. Please enter a number between 1 and 7.\n";
            continue;
        }

        clock_t start, end;
        double time_taken;

        switch (choice) {
        case 1:
            cout << "Loading... Displaying Total Articles...\n";
            cout << "Total articles loaded: " << articleCount << "\n";
            break;

        case 2:
            cout << "Loading... Displaying First 5 Articles...\n";
            if (articleCount == 0) {
                cout << "Error: No articles loaded.\n";
            }
            else {
                int limit = (articleCount < 5) ? articleCount : 5;

                // ✅ Print header
                cout << left << setw(6) << "No." << "  "
                    << setw(80) << "Title" << "  "
                    << setw(6) << "Year" << "\n";
                cout << string(100, '-') << endl;

                for (int i = 0; i < limit; i++) {
                    string title = articles[i].title;
                    if (title.length() > 75) title = title.substr(0, 72) + "...";

                    cout << left << setw(6) << (i + 1) << "  "
                        << setw(80) << title << "  "
                        << setw(6) << extractYear(articles[i].date) << "\n";
                }
            }
            break;

        case 3:
            cout << "Loading... Sorting Articles by Year...\n";
            if (articleCount == 0) {
                cout << "Error: No articles loaded.\n";
            }
            else {
                start = clock();
                sortArticlesByYear(articles, articleCount);
                end = clock();
                time_taken = double(end - start) / CLOCKS_PER_SEC;
                cout << "✅ Sorting completed in " << time_taken << " seconds.\n";
            }
            break;

        case 4:
            cout << "Loading... Searching Articles by Year and Category...\n";
            if (articleCount == 0) {
                cout << "Error: No articles loaded.\n";
            }
            else {
                int year;
                cout << "Enter year (1. 2015 | 2. 2016 | 3. 2017): ";
                while (!(cin >> year) || year < 1 || year > 3) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Error: Invalid choice. Enter 1 (2015), 2 (2016), or 3 (2017): ";
                }
                year = 2014 + year; // Convert 1 → 2015, 2 → 2016, 3 → 2017

                displayCategories();
                int catChoice;
                cout << "Select a category (1-" << categoryCount << "): ";
                while (!(cin >> catChoice) || catChoice < 1 || catChoice > categoryCount) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Error: Invalid category. Select 1-" << categoryCount << ": ";
                }
                string category = categories[catChoice - 1];

                start = clock();
                bool found = false;

                // ✅ Table Header
                cout << left << setw(6) << "No." << "  "
                    << setw(80) << "Title" << "  "
                    << setw(6) << "Year" << "  "
                    << setw(15) << "Category" << "\n";
                cout << string(110, '-') << endl;

                int count = 0;
                for (int i = 0; i < articleCount; i++) {
                    if (extractYear(articles[i].date) == year && articles[i].subject == category) {
                        found = true;
                        string title = articles[i].title;
                        if (title.length() > 75) title = title.substr(0, 72) + "...";

                        cout << left << setw(6) << (++count) << "  "
                            << setw(80) << title << "  "
                            << setw(6) << extractYear(articles[i].date) << "  "
                            << setw(15) << articles[i].subject << "\n";
                    }
                }

                end = clock();
                time_taken = double(end - start) / CLOCKS_PER_SEC;

                if (!found) {
                    cout << "No articles found for \"" << category << "\" in " << year << ".\n";
                }

                cout << "✅ Search completed in " << time_taken << " seconds.\n";
            }
            break;

        case 5:
            cout << "Loading... Analyzing Fake Political News by Year...\n";
            if (articleCount == 0) {
                cout << "Error: No articles loaded.\n";
            }
            else {
                int year;
                cout << "Enter year (1. 2015 | 2. 2016 | 3. 2017): ";
                while (!(cin >> year) || year < 1 || year > 3) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Error: Invalid choice. Enter 1 (2015), 2 (2016), or 3 (2017): ";
                }
                year = 2014 + year;

                start = clock();
                countPoliticalFakeNewsMonthly(articles, articleCount, year);
                end = clock();
                time_taken = double(end - start) / CLOCKS_PER_SEC;
                cout << "✅ Analysis completed in " << time_taken << " seconds.\n";
            }
            break;

        case 6:
            cout << "Loading... Analyzing Most Frequent Words in Fake News Articles...\n";
            if (articleCount == 0) {
                cout << "Error: No articles loaded.\n";
            }
            else {
                start = clock();
                wordFrequencyGovernment(articles, articleCount);
                end = clock();
                time_taken = double(end - start) / CLOCKS_PER_SEC;
                cout << "✅ Analysis completed in " << time_taken << " seconds.\n";
            }
            break;

        case 7:
            cout << "Exiting program...\n";
            return;

        default:
            cout << "Error: Invalid choice. Please enter a number between 1 and 7.\n";
            break;
        }
    }
}


/**
 * @brief Lists all CSV files in the "Dataset" folder.
 */
void listDatasets() {
    cout << "Available datasets in 'Dataset' folder:\n";
    int index = 1;
    for (const auto& entry : fs::directory_iterator("Dataset")) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            cout << index++ << ". " << entry.path().filename().string() << endl;
        }
    }
}

/**
 * @brief The main function: loads CSV files and invokes the menu.
 */
int main() {
    cout << "Checking for datasets in the 'Dataset' folder...\n";
    listDatasets();

    while (true) {
        cout << "\nEnter the number of the dataset you want to load (or 0 to skip): ";
        int datasetChoice;
        if (!(cin >> datasetChoice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Invalid input. Please enter an integer.\n";
            continue;
        }
        if (datasetChoice == 0) {
            cout << "Skipping dataset loading.\n";
            break;
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
            cout << "Loading dataset: " << selectedDataset << "...\n";
            articleCount = loadCSV(selectedDataset, articles, articleCount);
            if (articleCount == 0) {
                cerr << "No valid data found in file: " << selectedDataset << endl;
            }
            else {
                cout << "Successfully loaded " << articleCount << " articles from " << selectedDataset << "!\n";
            }
            break;
        }
        else {
            cout << "Invalid dataset choice. Please select a valid dataset.\n";
        }
    }

    // Display total articles loaded
    cout << "\nTotal articles loaded: " << articleCount << "\n";
    menu();
    return 0;
}

