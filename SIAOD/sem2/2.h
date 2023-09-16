//
// Created by sejapoe on 9/15/23.
//

#ifndef SEM2_2_H
#define SEM2_2_H

#include <fstream>
#include <random>
#include <algorithm>
#include <cstring>
#include "enter.h"

struct Entry {
    uint32_t key;
    char name[128];
};

void task2gen() {
    std::cout << BLUE_FG << "Enter count of entries: ";
    int count = enter(isPositive);
    std::ofstream file("task2_" + std::to_string(count) + ".data", std::ios::binary | std::ios::out);

    std::random_device rd;
    std::mt19937 gen(rd());

    auto *entry = new Entry[count];

    for (int i = 0; i < count; ++i) {
        entry[i].key = ((uint32_t) gen()) % 900000 + 100000;
        std::strcpy(entry[i].name, ("item_" + std::to_string(i)).c_str());
    }
    file.write((char *) entry, (int64_t) sizeof(Entry) * (int64_t) count);
    file.close();
}

void task2checkUnique() {
    std::cout << BLUE_FG << "Enter count of entries (file should exist): ";
    int count = enter(isPositive);
    std::ifstream file("task2_" + std::to_string(count) + ".data", std::ios::binary | std::ios::in);

    auto *entry = new Entry[count];
    file.read((char *) entry, (int64_t) sizeof(Entry) * (int64_t) count);
    auto newEnd = std::unique(entry, entry + count, [](const Entry &a, const Entry &b) {
        return a.key == b.key;
    });
    if (newEnd == entry + count) {
        std::cout << GREEN_FG << "Keys are unique\n";
    } else {
        std::cout << RED_FG << "Keys are not unique\n";

    }
}

void task2linear() {
    std::cout << BLUE_FG << "Enter count of entries (file should exist): ";
    int count = enter(isPositive);
    std::ifstream file("task2_" + std::to_string(count) + ".data", std::ios::binary | std::ios::in);

    std::cout << BLUE_FG << "Enter key (6-digit number): ";
    int key = enter(isPositive);


    auto runtime = measureTime([&]() {
        auto *entry = new Entry[count];
        file.read((char *) entry, (int64_t) sizeof(Entry) * (int64_t) count);
        std::string name;
        for (int i = 0; i < count; i++) {
            if (key == entry[i].key) {
                std::cout << BLUE_FG << "Result: " << name << "\n";
                return;
            }
        }
        std::cout << RED_FG << "Key not found\n";
    });
    std::cout << "\n" << RED_FG_BOLD << "Runtime: " << runtime << "ms" << "\n";
}

void task2binary() {
    std::cout << BLUE_FG << "Enter count of entries (file should exist): ";
    int count = enter(isPositive);
    std::ifstream file("task2_" + std::to_string(count) + ".data", std::ios::binary | std::ios::in);

    std::cout << BLUE_FG << "Enter key (6-digit number): ";
    int key = enter(isPositive);

    auto runtime = measureTime([&]() {
        auto *entry = new Entry[count];
        file.read((char *) entry, (int64_t) sizeof(Entry) * (int64_t) count);

        bool (*entryComparator)(const Entry &, const Entry &) = [](const Entry &a, const Entry &b) {
            return a.key < b.key;
        };
        std::sort(entry, entry + count, entryComparator);

        // binary search
        Entry searchEntry = {static_cast<uint32_t>(key), ""};
        auto res = std::lower_bound(entry, entry + count, searchEntry, entryComparator);

        if (res != entry + count && res->key == key) {
            std::cout << BLUE_FG << "Result: " << res->name << "\n";
        } else {
            std::cout << RED_FG << "Key not found\n";
        }
    });
    std::cout << "\n" << RED_FG_BOLD << "Runtime: " << runtime << "ms" << "\n";
}

#endif //SEM2_2_H
