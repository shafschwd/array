#ifndef UTILS_H
#define UTILS_H

#include <string>

/**
 * @brief Converts a string to lowercase in-place.
 */
inline void toLowerCase(std::string& s) {
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] + ('a' - 'A');
        }
    }
}

/**
 * @brief Removes punctuation from a string in-place.
 */
inline void removePunctuation(std::string& s) {
    std::string result;
    for (char c : s) {
        // Keep alphanumeric and basic whitespace
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == ' ')
        {
            result.push_back(c);
        }
    }
    s = result;
}

#endif // UTILS_H
