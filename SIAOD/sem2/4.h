//
// Created by sejapoe on 10/6/23.
//

#ifndef SEM2_4_H
#define SEM2_4_H

#include <string>
#include <sstream>
#include <iostream>
#include "colors.h"

bool isEndOfSentence(char c) {
    return c == '.' || c == '!' || c == '?';
}

bool isPunctuation(char c) {
    return c == ',' || isEndOfSentence(c);
}

std::string transformSentence(const std::string &text) {
    std::stringstream result;
    int spaceCounter = 0;
    bool wasPreviousSymbolPunctuation = false;
    for (char c: text) {
        if (c == ' ') {
            if (++spaceCounter == 1) result << c;
        } else {
            spaceCounter = 0;
            if (wasPreviousSymbolPunctuation) {
                result << " ";
                spaceCounter++;
            }
            wasPreviousSymbolPunctuation = isPunctuation(c);
            result << c;
            if (isEndOfSentence(c)) result << " ";
        }
    }
    return result.str();
}

void task4Point1() {
    std::cout << BLUE_FG << "Enter words with punctuation: " << RESET << "\n";
    std::string str;
    std::getline(std::cin, str);
    std::cout << GREEN_FG << "Result:" << RESET << "\n" << transformSentence(str);
}

void prefix_function(std::string s, int *arr) {
    int n = (int) s.length();
    for (int i = 1; i < n; i++) {
        int j = arr[i - 1];
        while (j > 0 && s[i] != s[j])
            j = arr[j - 1];
        if (s[i] == s[j])
            j++;
        arr[i] = j;
    }
}

void task4Point2() {
    std::cout << BLUE_FG << "Enter string to calculate prefix function:\n" << RESET;
    std::string str;
    std::getline(std::cin, str);
    unsigned long n = str.length();
    int *pi = new int[n]{0};
    prefix_function(str, pi);
    std::cout << GREEN_FG << "Result: \n";
    for (int i = 0; i < n; i++) {
        std::cout << pi[i] << (i != n - 1 ? ", " : "\n");
    }
}


#endif //SEM2_4_H
