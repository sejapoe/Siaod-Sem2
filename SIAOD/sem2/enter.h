//
// Created by sejapoe on 9/6/23.
//

#ifndef SEM2_ENTER_H
#define SEM2_ENTER_H

#include <functional>
#include <iostream>
#include <sstream>
#include "colors.h"

std::function<bool(int)> const isPositive = [](int a) { return a > 0; };
std::function<bool(double)> const anyDouble = [](double a) { return true; };
std::function<bool(int)> const anyInt = [](int a) { return true; };

template<typename T>
T enter(std::function<bool(T)> foo) {
    T t;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream ss(line);
        if ((ss >> t) && ss.eof() && foo(t)) {
            break;
        }
        std::cout << RED_FG << "Invalid input. Try again: ";
    }
    return t;
}

template<typename F>
long measureTime(F &&lambda) {
    auto start = std::chrono::high_resolution_clock::now();
    lambda();
    auto stop = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
}

#endif //SEM2_ENTER_H
