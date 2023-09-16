#include <iostream>
#include <map>
#include <functional>
#include <chrono>
#include "colors.h"
#include "enter.h"
#include "1.h"
#include "2.h"

using std::string;
using std::pair;
using std::map;
using std::cout;
using std::cin;


int main() {
    map<string, pair<string, void (*)()>> functions{
            {"1.1.a",   {"5 bit v 0",                       task1Point1A}},
            {"1.1.b",   {"7 bit v 1",                       task1Point1B}},
            {"1.1.c",   {"idk",                             task1Point1C}},
            {"1.2.a",   {"bit array up to 8",               task1Point2A}},
            {"1.2.b",   {"bit array up to 64",              task1Point2B}},
            {"1.2.c",   {"bit array up to 64, using array", task1Point2C}},
            {"1.3.gen", {"generate file for 1.3",           task1Point3Gen}},
            {"1.3.a",   {"sort file with 10^7 numbers",     task1Point3A}},
            {"2.gen",   {"generate file",                   task2gen}},
            {"2.uniq",  {"check keys for unique",           task2checkUnique}},
            {"2.lin",   {"linear search by key",            task2linear}},
            {"2.bin",   {"binary search by key",            task2binary}},
    };
    while (true) {
        cout << BLUE_FG_BOLD << "Enter task number (e.g. 1.1.a): ";

        std::function<bool(string)> isValidTask = [&functions](const string &s) {
            return s == "list" || functions.find(s) != functions.end();
        };
        string s = enter(isValidTask);

        if (s == "list") {
            for (const auto &func: functions) {
                cout << RESET << GREEN_FG << func.first << "\t" << func.second.first << "\n";
            }
            continue;
        }

        auto it = functions.find(s);

        char c = 'r';
        while (c == 'r' || c == 'n') {
            cout << BOLD << GREEN_FG << it->first << ": " << it->second.first << "\n"
                 << RESET << "\n";
            long runtime = measureTime([&it]() {
                it->second.second();
            });
            cout << "\n" << RED_FG_BOLD << "Runtime: " << runtime << "ms" << "\n"
                 << BLUE_FG_BOLD << "[e]xit, [r]estart, [n]ext or [c]hoose (default)? ";
            cin >> c;
            cin.ignore(32767, '\n');

            if (c == 'n' && ++it == functions.end()) {
                it = functions.begin();
            }
        }

        if (c == 'e') break;
    }
    cout << RED_FG_BOLD << "Good bye!\n" << RESET;
}