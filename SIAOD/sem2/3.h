//
// Created by sejapoe on 10/6/23.
//

#ifndef SEM2_3_H
#define SEM2_3_H

#include <string>
#include <ostream>
#include <iostream>
#include "colors.h"
#include "enter.h"

struct UniversitySpeciality {
    std::string specialityCode;
    std::string universityName;
};

class HashMap {
    const int polyHashBase = 13; // alphabet of key is 11 (0-9 and dot)

    int linStep = 7;
    int capacity;
    UniversitySpeciality **hashTable;

    [[nodiscard]] int hash(const std::string &s) const {
        int result = 0;
        int base = 1;
        for (char c: s) {
            int x = c == '.' ? 11 : (c - '0' + 1);
            result = (result + x * base) % capacity;
            base = (base * polyHashBase) % capacity;
        }
        std::cout << RED_BG << "===DEBUG===" << RESET << " hash(" << s << ") = " << result << " " << RED_BG
                  << "===DEBUG==="
                  << RESET "\n";
        return result;
    }

    [[nodiscard]] int index(const std::string &key) const {
        int keyHash = hash(key);
        int originalKeyHash = keyHash;
        while (true) {
            auto t = hashTable[keyHash];
            if (t == nullptr) return -1;
            if (t->specialityCode == key) return keyHash;

            keyHash = (keyHash + linStep) % capacity;
            if (keyHash == originalKeyHash) return -1;
        }
    }

public:
    explicit HashMap(int capacity) : capacity(capacity), hashTable(new UniversitySpeciality *[capacity]) {
        for (int i = 0; i < capacity; i++) {
            hashTable[i] = nullptr;
        }
    }

    HashMap() : HashMap(4) {}

    [[nodiscard]] UniversitySpeciality *get(const std::string &key) const {
        int index = this->index(key);
        return index == -1 ? nullptr : hashTable[index];
    }

    [[nodiscard]] bool remove(const std::string &key) {
        int index = this->index(key);
        if (index == -1) return false;
        auto t = hashTable[index];
        hashTable[index] = nullptr;
        delete t;
        return true;
    }

    void put(const std::string &key, const std::string &value) {
        int keyHash = hash(key);
        int originalKeyHash = keyHash;
        while (hashTable[keyHash] != nullptr) {
            keyHash = (keyHash + linStep) % capacity;
            if (originalKeyHash == keyHash) {
                resize();
                put(key, value);
                return;
            }
        }
        hashTable[keyHash] = new UniversitySpeciality{key, value};
    }

    void resize() {
        int oldCapacity = capacity;
        auto oldHashTable = hashTable;

        capacity *= 2;
        hashTable = new UniversitySpeciality *[capacity];

        std::cout << RED_BG << "===DEBUG===" << RESET << " resize from " << oldCapacity << " to " << capacity << " "
                  << RED_BG << "===DEBUG===" << RESET "\n";
        for (int i = 0; i < capacity; i++) {
            hashTable[i] = nullptr;
        }

        for (int i = 0; i < oldCapacity; i++) {
            auto t = oldHashTable[i];
            put(t->specialityCode, t->universityName);
            delete t;
        }
        delete[] oldHashTable;
    }

    friend std::ostream &operator<<(std::ostream &os, const HashMap &map) {
        os << "{";
        for (int i = 0; i < map.capacity; i++) {
            auto t = map.hashTable[i];
            if (t == nullptr) continue;
            os << t->specialityCode << ": " << t->universityName << (i != map.capacity - 1 ? ", " : "");
        }
        os << "}";
        return os;
    }

    virtual ~HashMap() {
        for (int i = 0; i < capacity; i++) {
            delete hashTable[i];
        }
        delete[] hashTable;
    }
};

void task3() {
    HashMap hashMap;
    // preload
    hashMap.put("09.03.04", "РТУ МИРЭА");
    hashMap.put("09.03.01", "ХИХИГС");
    hashMap.put("12.12.12", "ПГУ");
    hashMap.put("66.66.66", "МГУ");
    hashMap.put("77.77.77", "МГИМО");
    hashMap.put("01.03.05", "МФТИ");
    while (true) {
        std::cout << BLUE_FG_BOLD << "[p]rint, [a]dd, [g]et, [r]emove, [e]xit\n" << RESET;
        char c;
        std::cin >> c;
        std::cin.clear();
        std::cin.ignore();
//        std::flush(std::cin);
        if (!(c == 'p' || c == 'a' || c == 'g' || c == 'r' || c == 'e')) continue;
        if ('e' == c) return;
        if ('p' == c) std::cout << hashMap << "\n";
        else {
            std::cout << GREEN_FG << "Enter key like 99.99.99 (you can use digits or dots):\n";
            std::string key;
            std::getline(std::cin, key);
            if (!std::all_of(key.begin(), key.end(), [](char c) {
                return (c == '.' || c >= '0' && c <= '9');
            })) {
                std::cout << RED_FG << "Wrong key format\n";
                continue;
            }
            if ('g' == c) {
                auto t = hashMap.get(key);
                if (t == nullptr) {
                    std::cout << RED_FG << "No element with key " << key << "\n";
                } else {
                    std::cout << t->universityName << "\n";
                }
            }
            if ('r' == c) {
                if (hashMap.remove(key)) {
                    std::cout << GREEN_FG << "Deleted\n";
                } else {
                    std::cout << RED_FG << "No element with key " << key << "\n";
                }
            }
            if ('a' == c) {
                std::cout << GREEN_FG << "Enter value:\n";
                auto value = enter<std::string>([](const std::string &) { return true; });
                hashMap.put(key, value);
            }
        }
    }

}

#endif //SEM2_3_H
