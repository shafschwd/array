#include <iostream>
#include <filesystem>
#include <limits>
#include <ctime>
#include <chrono>
#include <iomanip>
#include "NewsArticle.h"
#include "Sorting.h"
#include "Searching.h"
#include "Analysis.h"

#define MAX_NEWS 50000

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

NewsArticle* articles = nullptr;  // Pointer for dynamic allocation
int articleCount = 0; // Current article count

const string categories[] = { "politics", "business", "entertainment", "health", "science", "sports", "technology" };
const int categoryCount = sizeof(categories) / sizeof(categories[0]);

void displayCategories() {
    cout << "\nAvailable Categories:\n";
    for (int i = 0; i < categoryCount; i++) {
        cout << " " << (i + 1) << ". " << categories[i] << "\n";
    }
}

void loadDataset(NewsArticle* articles, int& articleCount) {
    articleCount = 0;  // Reset counter at start

    // Load True.csv
    std::cout << "Loading True.csv...\n";
    int countTrue = loadCSV("Dataset/True.csv", articles, articleCount);
    std::cout << "✅ Loaded " << countTrue << " articles from True.csv\n";

    // Load Fake.csv
    std::cout << "Loading Fake.csv...\n";
    int countFake = loadCSV("Dataset/Fake.csv", articles, articleCount);
    std::cout << "✅ Loaded " << countFake << " articles from Fake.csv\n";

    // Only show the total articles count once
    std::cout << "\nSummary:\n";
    std::cout << "- True articles: " << countTrue << "\n";
    std::cout << "- Fake articles: " << countFake << "\n";
    // std::cout << "- Total articles: " << articleCount << "\n";
}

void listDatasets() {
    cout << "Available datasets:\n";
    int index = 1;
    for (const auto& entry : fs::directory_iterator("Dataset")) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            cout << index++ << ". " << entry.path().filename().string() << endl;
        }
    }
}

void menu(NewsArticle* articles, int& articleCount) {
    while (true) {
        cout << "\n========== MENU ==========\n"
             << "1. Display Total Articles\n"
             << "2. Display First 5 Articles\n"
             << "3. Sort Articles by Year\n"
             << "4. Search Articles by Year and Category\n"
             // << "5. Search for Articles by Keyword\n"
             << "5. Analyze Fake Political News by Year\n"
             << "6. Analyze Most Frequent Words in Fake News Article\n"
             << "7. Exit\n"
             << "===========================\n"
             << "Enter your choice: ";

        // std::cout << "DEBUG: articleCount before menu choice = " << articleCount << std::endl;
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Invalid input. Please enter a number between 1 and 9.\n";
            continue;
        }

        clock_t start, end;
        double time_taken;

        switch (choice) {
            case 1: {
                cout << "Loading... Displaying Total Articles...\n";

                int trueCount = 0, fakeCount = 0;
                for (int i = 0; i < articleCount; i++) {
                    int year = extractYear(articles[i].date);
                    if (year >= 2015 && year <= 2018) {  // Ensure valid year
                        if (i < 21417) {  // First file (True.csv) entries
                            trueCount++;
                        } else {          // Second file (Fake.csv) entries
                            fakeCount++;
                        }
                    }
                }

                cout << "\nArticle Distribution:\n";
                cout << "- True articles: " << trueCount << "\n";
                cout << "- Fake articles: " << fakeCount << "\n";
                // cout << "- Total articles: " << articleCount << "\n";
                break;
            }

            case 2:
                cout << "Loading... Displaying First 5 Articles...\n";
                if (articleCount == 0) {
                    cout << "Error: No articles loaded.\n";
                } else {
                    int limit = (articleCount < 5) ? articleCount : 5;

                    // Print header
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
                } else {
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
                } else {
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

                    // Table Header
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

            // case 5: {
            //     string keyword;
            //     cout << "Enter keyword to search for: ";
            //     cin.ignore();
            //     getline(cin, keyword);
            //     cout << "Searching for articles containing the keyword \"" << keyword << "\"...\n";
            //     searchKeyword(articles, articleCount, keyword);
            //     break;
            // }

            case 5:
                cout << "Loading... Analyzing Fake Political News by Year...\n";
            if (articleCount == 0) {
                cout << "Error: No articles loaded.\n";
            } else {
                int yearChoice;
                cout << "Choose the year:\n";
                cout << "1. 2015\n";
                cout << "2. 2016\n";
                cout << "3. 2017\n";
                cout << "4. 2018\n";
                while (!(cin >> yearChoice) || yearChoice < 1 || yearChoice > 4) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Error: Invalid choice. Enter 1 (2015), 2 (2016), 3 (2017), or 4 (2018): ";
                }
                int year = 2014 + yearChoice;

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
                } else {
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

int main() {
    int choice;

    // Allocate memory only once at program start
    articles = new NewsArticle[MAX_NEWS];
    articleCount = 0;

    while (true) {
        cout << "\n========== MENU ==========\n"
             << "1. Load Datasets (True.csv & Fake.csv)\n"
             << "0. Exit Program\n"
             << "==========================\n"
             << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Invalid input. Please enter 1 to load datasets or 0 to exit.\n";
            continue;
        }

        if (choice == 0) {
            cout << "Exiting program...\n";
            delete[] articles;  // Properly free memory before exit
            return 0;
        } else if (choice == 1) {
            cout << "Loading datasets...\n";

            // Start measuring time
            auto start = high_resolution_clock::now();

            // Load datasets
            loadDataset(articles, articleCount);

            // Stop measuring time
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - start);

            // Display total execution time
            cout << "Total Execution Time: " << duration.count() << " ms\n";

            if (articleCount == 0) {
                cerr << "No valid articles loaded. Exiting program.\n";
                delete[] articles;
                return 0;
            } else {
                break; // Proceed to menu after loading datasets
            }
        } else {
            cout << "Error: Invalid choice. Please enter 1 to load datasets or 0 to exit.\n";
        }
    }

    // Pass correct arguments to `menu()`
    menu(articles, articleCount);

    delete[] articles;
    return 0;
}