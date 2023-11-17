//
// Created by sejapoe on 10/13/23.
//

#ifndef SEM2_7_H
#define SEM2_7_H


#include <string>
#include <utility>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <locale>
#include <codecvt>
#include <queue>
#include "colors.h"

using convert_type = std::codecvt_utf8<wchar_t>;

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
void task7Point2A() {
    // output shit
    std::wstring_convert<convert_type, wchar_t> converter;


    std::wstring text = L"По-турецки говорили. Чяби, чяряби\nЧяряби, чяби-чяби. Мы набрали в рот воды.";
    std::cout << converter.to_bytes(text) << "\n\n";



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
    for (auto i: compressed) {
        t.push_back(i);
        auto toChar = codeToChar.find(t);
        if (toChar == codeToChar.end()) continue;
        t.clear();
        decompressed.push_back(toChar->second);
    }
    std::cout << GREEN_FG << "Decompressed to:" << RESET << "\n";
    std::cout << converter.to_bytes(decompressed);
    std::cout << "\n";
    if (text == decompressed) {
        std::cout << GREEN_FG << "Correct\n";
    } else {
        std::cout << RED_FG << "Incorrect\n";
    }
    std::cout << BLUE_FG << "Compression coefficient : "
              << ((double) compressed.size()) / ((double) text.size() * 4 * 8) << RESET << "\n";
}

struct Node {
    std::string value;
    int weight;
    Node *left;
    Node *right;


    Node(std::string value, int weight, Node *left, Node *right) : value(std::move(value)), weight(weight), left(left),
                                                                   right(right) {}

    Node(const std::string &value, int weight) : Node(value, weight, nullptr, nullptr) {}

    Node(Node *left, Node *right) : Node(left->value + right->value, left->weight + right->weight, left, right) {}

    void generateCodes(std::map<wchar_t, std::string> &map, std::wstring_convert<convert_type, wchar_t> &converter,
                       const std::string &buffer = "") {
        auto t = converter.from_bytes(value);
        if (t.length() == 1) {
            auto w = t[0];
            map.insert(std::make_pair(w, buffer));
            return;
        }
        if (left != nullptr) left->generateCodes(map, converter, buffer + "0");
        if (right != nullptr) right->generateCodes(map, converter, buffer + "1");
    }

    virtual ~Node() {
        delete left;
        delete right;
    }
};

struct CmpNodePointers {
    bool operator()(const Node *lhs, const Node *rhs) const {
        return lhs->weight > rhs->weight;
    }
};

// хаффман
void task7Point2B() {
    // output shit
    std::wstring_convert<convert_type, wchar_t> converter;


    std::wstring text = L"По-турецки говорили. Чяби, чяряби\nЧяряби, чяби-чяби. Мы набрали в рот воды.";
    std::cout << converter.to_bytes(text) << "\n\n";



    // preparing
    std::map<wchar_t, int> frequenciesMap;
    for (auto c: text) {
        int cnt = 0;
        auto t = frequenciesMap.find(c);
        if (t != frequenciesMap.end()) cnt = t->second;
        frequenciesMap.insert_or_assign(c, cnt + 1);
    }

    std::priority_queue<Node *, std::vector<Node *>, CmpNodePointers> pq;
    for (auto k: frequenciesMap) {
        auto node = new Node(converter.to_bytes(k.first), k.second);
        pq.push(node);
    }

    // create tree
    while (pq.size() > 1) {
        auto first = pq.top();
        pq.pop();
        auto second = pq.top();
        pq.pop();
        std::cout << first->weight << " " << second->weight << "\n";
        auto node = new Node(first, second);
        pq.push(node);
    }
    auto root = pq.top();
    pq.pop();

    // calculate codes
    std::map<wchar_t, std::string> charToCode;
    root->generateCodes(charToCode, converter);
    delete root;

//   print codes
//    for (auto &k : charToCodes) {
//        if (k.first == L'\n') {
//            std::cout << "\\n";
//        } else {
//            std::cout << converter.to_bytes(k.first);
//        }
//        std::cout << " : " << k.second << "\n";
//    }

    // compress
    std::string compressed;
    for (auto c: text) {
        auto code = charToCode.find(c);
        compressed += code->second;
    }

    std::cout << GREEN_FG << "Compressed to:" << RESET << "\n";
    for (auto i: compressed) {
        std::cout << i;
    }
    std::cout << "\n";

    // decompress
    std::map<std::string, wchar_t> codeToChar;
    std::transform(charToCode.begin(), charToCode.end(), std::inserter(codeToChar, codeToChar.end()), [](auto k) {
        return std::make_pair(k.second, k.first);
    });

    std::wstring decompressed;
    std::string t;
    for (auto i: compressed) {
        t.push_back(i);
        auto toChar = codeToChar.find(t);
        if (toChar == codeToChar.end()) continue;
        t.clear();
        decompressed.push_back(toChar->second);
    }
    std::cout << GREEN_FG << "Decompressed to:" << RESET << "\n";
    std::cout << converter.to_bytes(decompressed);
    std::cout << "\n";
    if (text == decompressed) {
        std::cout << GREEN_FG << "Correct\n";
    } else {
        std::cout << RED_FG << "Incorrect\n";
    }
    std::cout << BLUE_FG << "Compression coefficient : "
              << ((double) compressed.size()) / ((double) text.size() * 4 * 8) * 100 << "%" << RESET << "\n";
}

struct Pair {
    int offset;
    int len;
    char symbol;
};

// lz77
void task7lz77() {
    const std::string &phrase = "010110110110100010001";

    std::vector<Pair> encoded;

    int phraseLength = phrase.length();
    int currentIndex = 0;

    while (currentIndex < phraseLength) {
        Pair currentPair;
        currentPair.offset = 0;
        currentPair.len = 0;
        currentPair.symbol = phrase[currentIndex];

        for (int i = currentIndex - 1; i >= 0; i--) {
            int j = 0;
            while (i + j < currentIndex && phrase[i + j] == phrase[currentIndex + j] &&
                   currentIndex + j < phraseLength) {
                j++;
            }

            if (j > currentPair.len) {
                currentPair.offset = currentIndex - i - 1;
                currentPair.len = j;
                if (currentIndex + j < phraseLength) {
                    currentPair.symbol = phrase[currentIndex + j];
                }
            }
        }

        encoded.push_back(currentPair);
        if (currentPair.len == 0) {
            currentIndex += 1;
        } else {
            currentIndex += currentPair.len;
        }
    }

    for (const Pair &pair: encoded) {
        std::cout << "<" << pair.offset << ", " << pair.len << ", " << pair.symbol << "> ";
    }
    std::cout << std::endl;
}

struct SymbolCode {
    int index;
    std::wstring symbol;
    wchar_t character;

    SymbolCode(int index, std::wstring symbol) : index(index), symbol(std::move(symbol)), character('\0') {}

    SymbolCode(int index, wchar_t character) : index(index), character(character) {}
};

bool findInDictionary(const std::wstring &target, const std::vector<SymbolCode *> &dictionary, int &index) {
    for (const auto &entry: dictionary) {
        if (target == entry->symbol) {
            index = entry->index;
            return true;
        }
    }
    return false;
}

void task7lz78() {
    std::wstring_convert<convert_type, wchar_t> converter;
    const std::wstring &phrase = L"менменаменаменатеп";

    std::wstring currentSubstring;
    std::vector<SymbolCode *> dictionary;
    std::vector<SymbolCode *> encodedResult;
    int index = 0;

    for (wchar_t character: phrase) {
        currentSubstring += character;
        if (!findInDictionary(currentSubstring, dictionary, index)) {
            dictionary.push_back(new SymbolCode(dictionary.size() + 1, currentSubstring));
            encodedResult.push_back(new SymbolCode(index, character));
            index = 0;
            currentSubstring = L"";
        }
    }

    for (const auto &entry: encodedResult) {
        std::cout << "<" << entry->index << ", " << converter.to_bytes(entry->character) << "> ";
    }

}

#endif //SEM2_7_H
