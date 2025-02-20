#ifndef UTILS_H
#define UTILS_H

#include <string>

// Convert a string to lowercase
inline void toLowerCase(std::string &s) {
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] + ('a' - 'A');
        }
    }
}

// Remove punctuation from a string
inline void removePunctuation(std::string &s) {
    std::string result = "";
    for (size_t i = 0; i < s.length(); i++) {
        if (!(s[i] >= 33 && s[i] <= 47) &&  // ASCII range for most punctuation
            !(s[i] >= 58 && s[i] <= 64) &&
            !(s[i] >= 91 && s[i] <= 96) &&
            !(s[i] >= 123 && s[i] <= 126)) {
            result += s[i];
            }
    }
    s = result;
}

#endif // UTILS_H
