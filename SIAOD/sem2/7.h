//
// Created by sejapoe on 10/13/23.
//

#ifndef SEM2_7_H
#define SEM2_7_H


#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
#include <iostream>
#include "colors.h"

void calculateCodes(std::vector<std::tuple<wchar_t, int, std::vector<int>>>::iterator begin,
                    std::vector<std::tuple<wchar_t, int, std::vector<int>>>::iterator end) {
    if (std::distance(begin, end) == 1) return;
    int leftSum = 0, index = 0, rightSum = std::accumulate(begin, end, 0,
                                                           [](int cur, auto k) { return cur + std::get<1>(k); });
    while (leftSum < rightSum) {
        index++;
        leftSum += std::get<1>(begin[index]);
        rightSum -= std::get<1>(begin[index]);
    }
    char addition = 1;
    for (auto it = begin; it < end; it++) {
        if (std::distance(begin, it) == index) addition = 0;
        std::get<2>(*it).push_back(addition);
    }
    calculateCodes(begin, begin + index);
    calculateCodes(begin + index, end);
}

// шеннон-фано
void task7Point1Point1() {
    std::wcout.imbue(std::locale());
    std::wcout << L"Шеннон-фано\n";
    std::wstring text = L"По-турецки говорили. Чяби, чяряби\nЧяряби, чяби-чяби. Мы набрали в рот воды.";
    std::wcout << L"Как дела? " << " Нормально\n" << text << "\n\n";
    std::wcout << "\n\n";

    // preparing
    std::map<wchar_t, int> frequenciesMap;
    for (auto c: text) {
        int cnt = 0;
        auto t = frequenciesMap.find(c);
        if (t != frequenciesMap.end()) cnt = t->second;
        frequenciesMap.insert_or_assign(c, cnt + 1);
    }

    std::vector<std::pair<wchar_t, int>> frequencies(frequenciesMap.begin(), frequenciesMap.end());
    std::sort(frequencies.begin(), frequencies.end(), [](auto lhs, auto rhs) {
        return lhs.second < rhs.second;
    });

    std::vector<std::tuple<wchar_t, int, std::vector<int>>> frequenciesWithCode(frequencies.size());
    std::transform(frequencies.begin(), frequencies.end(), frequenciesWithCode.begin(), [](auto e) {
        return std::make_tuple(e.first, e.second, std::vector<int>());
    });

    calculateCodes(frequenciesWithCode.begin(), frequenciesWithCode.end());
    std::map<wchar_t, std::vector<int>> charToCode;
    std::transform(frequenciesWithCode.begin(), frequenciesWithCode.end(), std::inserter(charToCode, charToCode.end()),
                   [](auto k) { return std::make_pair(std::get<0>(k), std::get<2>(k)); });


    // compress
    std::vector<int> compressed;
    for (auto c: text) {
        auto code = charToCode.find(c);
        std::copy(code->second.begin(), code->second.end(), std::inserter(compressed, compressed.end()));
    }

    std::cout << text.size() * 4 * 8 << " bits to " << compressed.size() << "\n";

    std::cout << GREEN_FG << "Compressed to:" << RESET << "\n";
    for (auto i: compressed) {
        std::cout << i;
    }
    std::cout << "\n";

    // decompress
    std::map<std::vector<int>, wchar_t> codeToChar;
    std::transform(charToCode.begin(), charToCode.end(), std::inserter(codeToChar, codeToChar.end()), [](auto k) {
        return std::make_pair(k.second, k.first);
    });

    std::wstring decompressed;
    std::vector<int> t;
    for (auto i : compressed) {
        t.push_back(i);
        auto toChar = codeToChar.find(t);
        if (toChar == codeToChar.end()) continue;
        t.clear();
        decompressed.push_back(toChar->second);
    }
    std::wcout << GREEN_FG << "Decompressed to:" << RESET << "\n";
    std::wcout << decompressed;
    std::wcout << "\n";
    if (text == decompressed) {
        std::wcout << GREEN_FG << "Correct\n";
    } else {
        std::wcout << RED_FG << "Incorrect\n";
    }
}

#endif //SEM2_7_H
