#include "Searching.h"
#include <iostream>

using namespace std;

void searchKeyword(NewsArticle* arr, int size, const string& keyword) {
    string lowerKeyword = keyword;
    transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);

    bool found = false;
    for (int i = 0; i < size; i++) {
        string lowerTitle = arr[i].title;
        string lowerText = arr[i].text;
        transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
        transform(lowerText.begin(), lowerText.end(), lowerText.begin(), ::tolower);

        if (lowerTitle.find(lowerKeyword) != string::npos || lowerText.find(lowerKeyword) != string::npos) {
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

