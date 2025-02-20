#include <iostream>
#include <string>
#include "NewsArticle.h"

using namespace std;

void toLowerCase(string& str) {
    for (char& c : str) {
        c = tolower(c);
    }
}

bool isDelimiter(char c) {
    return !isalpha(c);
}

void searchKeyword(NewsArticle* arr, int size, const string& keyword) {
    string lowerKeyword = keyword;
    toLowerCase(lowerKeyword);

    bool found = false;
    for (int i = 0; i < size; i++) {
        string lowerTitle = arr[i].title;
        string lowerText = arr[i].text;
        toLowerCase(lowerTitle);
        toLowerCase(lowerText);

        // Tokenize the title and text
        string word;
        bool keywordFound = false;

        for (char c : lowerTitle) {
            if (isDelimiter(c)) {
                if (word == lowerKeyword) {
                    keywordFound = true;
                    break;
                }
                word.clear();
            } else {
                word += c;
            }
        }
        if (!keywordFound) {
            for (char c : lowerText) {
                if (isDelimiter(c)) {
                    if (word == lowerKeyword) {
                        keywordFound = true;
                        break;
                    }
                    word.clear();
                } else {
                    word += c;
                }
            }
        }

        if (keywordFound) {
            cout << "Title: " << arr[i].title << endl;
            cout << "Text: " << arr[i].text << endl;
            cout << "--------------------------" << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No articles found containing the keyword \"" << keyword << "\"." << endl;
    }
}