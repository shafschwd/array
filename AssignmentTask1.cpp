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


// Global variables for True & Fake counts
// ✅ Correct usage: Only declare, do not define.
extern int globalCountTrue;  // ✅ Only declare it here
int globalCountFake = 0;  // ✅ This is fine because it's not duplicated elsewhere


void loadDataset(NewsArticle* articles, int& articleCount) {
    articleCount = 0;  // Reset counter at start

    std::cout << "Loading True.csv...\n";
    globalCountTrue = loadCSV("Dataset/True.csv", articles, articleCount);

    std::cout << " Loaded " << globalCountTrue << " articles from True.csv\n";
    std::cout << "Loading Fake.csv...\n";
    globalCountFake = loadCSV("Dataset/Fake.csv", articles, articleCount);
    std::cout << " Loaded " << globalCountFake << " articles from Fake.csv\n";

    std::cout << "\nSummary:\n";
    std::cout << "- True articles: " << globalCountTrue << "\n";
    std::cout << "- Fake articles: " << globalCountFake << "\n";
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
        while (true) {
            cout << "Enter your choice: ";
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Error: Invalid input. Please enter a number between 1 and 7.\n";
            }
            else if (choice >= 1 && choice <= 7) {
                break; // ✅ Valid input, exit loop
            }
            else {
                cout << "Error: Invalid choice. Enter a number between 1 and 7.\n";
            }
        }

        clock_t start, end;
        double time_taken;

        switch (choice) {
            case 1: {
                cout << "Loading... Displaying Total Articles...\n";

                auto start = high_resolution_clock::now();  // Start timing

                int trueCount = 0, fakeCount = 0;

                for (int i = 0; i < articleCount; i++) {
                    int year = extractYear(articles[i].date);

                    if (year >= 2015 && year <= 2018) {
                        if (i < globalCountTrue) {
                            trueCount++;
                        }
                        else {
                            fakeCount++;
                        }
                    }
                }

                cout << "\nTotal True News Articles: " << trueCount << "\n";
                cout << "Total Fake News Articles: " << fakeCount << "\n";

                auto end = high_resolution_clock::now();  // End timing
                auto duration = duration_cast<milliseconds>(end - start);
                cout << "\nExecution Time: " << duration.count() << " ms\n";

                break;
            }


        case 2: {
            cout << "Loading... Displaying First 5 Articles...\n";

            if (articleCount == 0) {
                cout << "Error: No articles loaded.\n";
                break;
            }         

            // ---------------------------
            // Display first 5 TRUE News
            // ---------------------------
            start = clock();
            cout << "\n[TrueNews]\n";
            cout << string(85, '=') << "\n";  // Add == below [FakeNews]
            cout << "No.  Title" << string(57, ' ') << "Date        Category\n";
            cout << string(85, '=') << "\n";

            int limitTrue = (globalCountTrue < 5) ? globalCountTrue : 5;
            for (int i = 0; i < limitTrue; i++) {
                string title = articles[i].title;
                if (title.size() > 50) title = title.substr(0, 47) + "..."; // Keep title length consistent

                cout << left << setw(4) << (i + 1) << " "
                    << setw(60) << title
                    << setw(12) << articles[i].date
                    << articles[i].subject << "\n";
            }
            cout << string(85, '=') << "\n";  // Add == at the end before Execution Time
            end = clock();
            time_taken = double(end - start) / CLOCKS_PER_SEC * 1000.0;
            cout << "\nExecution Time: " << (int)time_taken << " ms\n\n";


            // ---------------------------
            // Display first 5 FAKE News
            // ---------------------------
            start = clock();
            cout << "[FakeNews]\n";
            cout << string(85, '=') << "\n";  // Add == below [FakeNews]
            cout << "No.  Title" << string(57, ' ') << "Date        Category\n";
            cout << string(85, '=') << "\n";

            int limitFake = (globalCountFake < 5) ? globalCountFake : 5;
            int fakeStart = globalCountTrue;  // Fake articles start after True articles
            for (int i = 0; i < limitFake; i++) {
                int idx = fakeStart + i;
                string title = articles[idx].title;
                if (title.size() > 50) title = title.substr(0, 47) + "...";

                cout << left << setw(4) << (i + 1) << " "
                    << setw(60) << title
                    << setw(12) << articles[idx].date
                    << articles[idx].subject << "\n";
            }
            cout << string(85, '=') << "\n";  // Add == at the end before Execution Time
            end = clock();
            time_taken = double(end - start) / CLOCKS_PER_SEC * 1000.0;
            cout << "\nExecution Time: " << (int)time_taken << " ms\n";

            break;
        }



        case 3:
            cout << "Loading... Sorting Articles by Year...\n";

            if (articleCount == 0) {
                cout << "Error: No articles loaded.\n";
            }
            else {
                auto start = chrono::high_resolution_clock::now(); // Start timing in milliseconds

                sortAndDisplaySeparate(articles, articleCount);

                auto end = chrono::high_resolution_clock::now(); // End timing
                auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count(); // Convert to ms

                cout << "✅ Sorting completed in " << duration << " ms.\n";
            }
            break;


        case 4: {
            cout << "Loading... Searching Articles by Year and Category...\n";

            if (articleCount == 0) {
                cout << "Error: No articles loaded.\n";
                break;
            }

            // Step 1: Ask user to select dataset (True, Fake, or Both)
            int datasetChoice;
            cout << "\nChoose dataset to search:\n";
            cout << "1. Search True News only\n";
            cout << "2. Search Fake News only\n";
            cout << "3. Search Both\n";
            cout << "4. Back to Menu\n";
            cout << "Enter your choice: ";

            while (!(cin >> datasetChoice) || datasetChoice < 1 || datasetChoice > 4) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Error: Invalid choice. Enter 1 (True), 2 (Fake), 3 (Both), or 4 (Back): ";
            }
            if (datasetChoice == 4) break;

            // Step 2: Determine available years
            int availableYears[100];
            int yearCount = 0;

            for (int i = 0; i < articleCount; i++) {
                int year = extractYear(articles[i].date);
                bool alreadyAdded = false;

                for (int j = 0; j < yearCount; j++) {
                    if (availableYears[j] == year) {
                        alreadyAdded = true;
                        break;
                    }
                }

                if (!alreadyAdded) {
                    if ((datasetChoice == 1 && i < globalCountTrue) ||
                        (datasetChoice == 2 && i >= globalCountTrue) ||
                        datasetChoice == 3) {
                        availableYears[yearCount++] = year;
                    }
                }
            }

            // Step 3: Sort years in ascending order
            for (int i = 0; i < yearCount - 1; i++) {
                for (int j = i + 1; j < yearCount; j++) {
                    if (availableYears[i] > availableYears[j]) {
                        swap(availableYears[i], availableYears[j]);
                    }
                }
            }

            // Display available years
            cout << "\nAvailable years (Select a number):\n";
            for (int i = 0; i < yearCount; i++) {
                cout << (i + 1) << ". " << availableYears[i] << "\n";
            }

            // Step 4: Ask user to select a year
            int yearChoice;
            cout << "Enter your choice: ";
            while (!(cin >> yearChoice) || yearChoice < 1 || yearChoice > yearCount) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Error: Invalid choice. Enter a valid number: ";
            }
            int selectedYear = availableYears[yearChoice - 1];

            // Step 5: Determine available categories for the selected year
            string trueCategories[100], fakeCategories[100];
            int trueCount = 0, fakeCount = 0;

            for (int i = 0; i < articleCount; i++) {
                if (extractYear(articles[i].date) == selectedYear) {
                    bool alreadyAdded = false;

                    if (i < globalCountTrue) {
                        for (int j = 0; j < trueCount; j++) {
                            if (trueCategories[j] == articles[i].subject) {
                                alreadyAdded = true;
                                break;
                            }
                        }
                        if (!alreadyAdded) {
                            trueCategories[trueCount++] = articles[i].subject;
                        }
                    }
                    else {
                        for (int j = 0; j < fakeCount; j++) {
                            if (fakeCategories[j] == articles[i].subject) {
                                alreadyAdded = true;
                                break;
                            }
                        }
                        if (!alreadyAdded) {
                            fakeCategories[fakeCount++] = articles[i].subject;
                        }
                    }
                }
            }

            // Step 6: Display relevant categories based on dataset choice
            if (datasetChoice == 1) {  // ✅ True News only
                cout << "\nAvailable categories for True News:\n";
                if (trueCount == 0) {
                    cout << "No categories available in True dataset for " << selectedYear << ".\n";
                    break;
                }
                for (int i = 0; i < trueCount; i++) {
                    cout << (i + 1) << ". " << trueCategories[i] << "\n";
                }
            }
            else if (datasetChoice == 2) {  // ✅ Fake News only
                cout << "\nAvailable categories for Fake News:\n";
                if (fakeCount == 0) {
                    cout << "No categories available in Fake dataset for " << selectedYear << ".\n";
                    break;
                }
                for (int i = 0; i < fakeCount; i++) {
                    cout << (i + 1) << ". " << fakeCategories[i] << "\n";
                }
            }
            else {  // ✅ Both datasets
                cout << "\nAvailable categories for True News:\n";
                for (int i = 0; i < trueCount; i++) {
                    cout << (i + 1) << ". " << trueCategories[i] << "\n";
                }

                cout << "\nAvailable categories for Fake News:\n";
                for (int i = 0; i < fakeCount; i++) {
                    cout << (trueCount + i + 1) << ". " << fakeCategories[i] << "\n";
                }
            }

            if (trueCount == 0 && fakeCount == 0) {
                cout << "No available categories for the selected year. Returning to menu.\n";
                break;
            }

            // Step 7: Ask user to select a category
            int categoryChoice;
            cout << "\nEnter your choice: ";
            while (!(cin >> categoryChoice) || categoryChoice < 1 || categoryChoice >(trueCount + fakeCount)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Error: Invalid choice. Enter a valid number: ";
            }

            string selectedCategory;
            if (categoryChoice <= trueCount) {
                selectedCategory = trueCategories[categoryChoice - 1];
            }
            else {
                selectedCategory = fakeCategories[categoryChoice - trueCount - 1];
            }

            // Step 8: Search for articles in the selected year & category
            auto start = std::chrono::high_resolution_clock::now();

            cout << "\n===================================================================\n";
            cout << " No.  | Title                                               | Year  | Category       \n";
            cout << "===================================================================\n";

            int resultCount = 0;
            for (int i = 0; i < articleCount; i++) {
                if (extractYear(articles[i].date) == selectedYear && articles[i].subject == selectedCategory) {
                    string title = articles[i].title;
                    if (title.length() > 50) title = title.substr(0, 47) + "...";
                    cout << setw(4) << (resultCount + 1) << " | " << setw(50) << left << title << " | "
                        << setw(6) << selectedYear << " | " << setw(15) << left << articles[i].subject << "\n";
                    resultCount++;
                }
            }
            cout << "-------------------------------------------------------------------\n";
            cout << "Total matching articles: " << resultCount << "\n";
            cout << "===================================================================\n";

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            cout << "Search Execution Time: " << duration << " ms\n";

            break;
        }



        case 5:
            cout << "Loading... Analyzing Fake Political News by Year...\n";
            if (articleCount == 0) {
                cout << "Error: No articles loaded.\n";
            }
            else {
                int yearChoice;
                cout << "Choose the year:\n";
                cout << "1. 2015\n";
                cout << "2. 2016\n";
                cout << "3. 2017\n";
                cout << "4. 2018\n";
                cout << "Enter your choice: ";
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
        }
        else if (choice == 1) {
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
            }
            else {
                break; // Proceed to menu after loading datasets
            }
        }
        else {
            cout << "Error: Invalid choice. Please enter 1 to load datasets or 0 to exit.\n";
        }
    }

    // Pass correct arguments to `menu()`
    menu(articles, articleCount);

    delete[] articles;
    return 0;
}
