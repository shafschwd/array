#include "Searching.h"
#include "NewsArticle.h"
#include <iostream>
#include <string>

using namespace std;

/**
 * @brief Converts a string to lowercase (inline version in Utils.h is also possible).
 */
void toLowerCaseLocal(string& str) {
    for (char& c : str) {
        c = tolower(c);
    }
}

/**
 * @brief Checks if a character is a delimiter (non-alphabetic).
 */
bool isDelimiter(char c) {
    return !isalpha(c);
}

/**
 * @brief Searches for a keyword in the title and text of articles.
 *
 * @param arr       Array of NewsArticle
 * @param size      Number of articles
 * @param keyword   Keyword to search for
 */
void searchKeyword(NewsArticle arr[], int size, const std::string& keyword) {
    if (size == 0) {
        cout << "Error: No articles loaded.\n";
        return;
    }

    string lowerKeyword = keyword;
    toLowerCaseLocal(lowerKeyword);

    bool foundAny = false;
    for (int i = 0; i < size; i++) {
        // Convert to lowercase for case-insensitive search
        string lowerTitle = arr[i].title;
        string lowerText = arr[i].text;
        toLowerCaseLocal(lowerTitle);
        toLowerCaseLocal(lowerText);

        bool found = false;
        // Tokenize the title
        {
            string word;
            for (char c : lowerTitle) {
                if (isDelimiter(c)) {
                    if (word == lowerKeyword) {
                        found = true;
                        break;
                    }
                    word.clear();
                }
                else {
                    word.push_back(c);
                }
            }
            // Check last word
            if (!found && word == lowerKeyword) {
                found = true;
            }
        }

        // Tokenize the text only if not found in title
        if (!found) {
            string word;
            for (char c : lowerText) {
                if (isDelimiter(c)) {
                    if (word == lowerKeyword) {
                        found = true;
                        break;
                    }
                    word.clear();
                }
                else {
                    word.push_back(c);
                }
            }
        }

        // If found, display the article
        if (found) {
            cout << "\nTitle: " << arr[i].title << "\n";
            cout << "Text: " << arr[i].text << "\n";
            cout << "--------------------------" << endl;
            foundAny = true;
        }
    }

    if (!foundAny) {
        cout << "No articles found containing the keyword \"" << keyword << "\".\n";
    }
}
    if (!found) {
        cout << "No articles found containing the keyword \"" << keyword << "\"." << endl;
    }
}
