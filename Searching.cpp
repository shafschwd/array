#include "Searching.h"
#include <iostream>

using namespace std;

void searchKeyword(NewsArticle arr[], int size, const std::string& keyword) {
    bool found = false;
    cout << "Articles containing keyword '" << keyword << "':\n";

    for (int i = 0; i < size; i++) {
        if (arr[i].title.find(keyword) != string::npos || arr[i].text.find(keyword) != string::npos) {
            cout << "- " << arr[i].title << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No articles found with the keyword.\n";
    }
}

