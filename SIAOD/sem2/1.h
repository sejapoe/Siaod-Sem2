//
// Created by sejapoe on 9/6/23.
//

#ifndef SEM2_1_H
#define SEM2_1_H

#include <bitset>
#include <random>
#include <fstream>

void task1Point1A() {
    std::cout << BLUE_FG << "Введите число (0-255): ";
    std::function<bool(int)> isUnsignedChar = [](int t) { return t >= 0 && t <= 255; };
    unsigned char x = enter(isUnsignedChar);
    x = x & (~(1 << 4));
    std::cout << (int) x << "\n";
}

void task1Point1B() {
    std::cout << BLUE_FG << "Введите число (0-255): ";
    std::function<bool(int)> isUnsignedChar = [](int t) { return t >= 0 && t <= 255; };
    unsigned char x = enter(isUnsignedChar);
    x = x | (1 << 6);
    std::cout << (int) x << "\n";
}

void task1Point1C() {
    unsigned int x = 25;
    const int n = sizeof(int) * 8; // = 32
    unsigned mask = (1 << (n - 1));
    std::cout << BLUE_FG << "Start mask: " << std::bitset<n>(mask) << "\n";
    std::cout << BLUE_FG << "Result: ";
    // Вывод 'x' в бинарном виде
    for (int i = 1; i <= n; ++i) {
        // оставляем только один бит (соответствующий текущему положению маски) и сдвигаем влево оставляя один бит (старший)
        std::cout << ((x & mask) >> (n - i));
        // сдвигаем маску влево
        mask >>= 1;
    }
    std::cout << "\n";
}

void task1Point2A() {
    std::cout << BLUE_FG << "Enter up to 8 numbers in range 0-7 in one line: \n";
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);

    unsigned char bitArray;
    int t;
    while (ss >> t) {
        bitArray |= (1 << t);
    }
    std::cout << BLUE_FG << "Result: ";
    int mask = 1;
    for (int i = 0; i < 8; ++i) {
        if ((bitArray & (1 << i)) != 0) {
            std::cout << i << " ";
        }
        mask <<= 1;
    }
    std::cout << "\n";
}

void task1Point2B() {
    std::cout << BLUE_FG << "Enter up to 64 numbers in range 0-63 in one line: \n";
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);

    uint64_t bitArray = 0;
    uint64_t t;
    while (ss >> t) {
        bitArray |= ((uint64_t) 1 << t);
    }
    std::cout << BLUE_FG << "Result: ";
    for (int i = 0; i < 64; ++i) {
        if ((bitArray & ((uint64_t) 1 << i)) != 0) {
            std::cout << i << " ";
        }
    }
    std::cout << "\n";
}

void task1Point2C() {
    std::cout << BLUE_FG << "Enter up to 64 numbers in range 0-63 in one line: \n";
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);

    unsigned char bitArray[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int t;
    while (ss >> t) {
        bitArray[t / 8] |= (1 << (t % 8));
    }
    std::cout << BLUE_FG << "Result: ";
    for (int i = 0; i < 64; ++i) {
        if ((bitArray[i / 8] & (1 << (i % 8))) != 0) {
            std::cout << i << " ";
        }
    }
    std::cout << "\n";
}

void task1Point3Gen() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::ofstream outputFile("task1_point3.txt");
    for (int i = 0; i < 10000000; ++i) {
        outputFile << ((uint16_t) gen()) << "\n";
    }
}

void task1Point3A() {
    uint64_t baseMask = 1;

    size_t n = sizeof(uint64_t) * 8;
    size_t size = UINT16_MAX / n + 1;
    uint64_t bitArray[size];

    std::ifstream inputFile("task1_point3.txt");
    uint32_t t;
    while (inputFile >> t) {
        bitArray[t / n] |= (baseMask << (t % n));
    }
    inputFile.close();

    std::ofstream outputFile("task1_point3_output.txt");
    for (uint32_t i = 0; i < UINT16_MAX; ++i) {
        if (bitArray[i / n] & (baseMask << (i % n))) {
            outputFile << i << "\n";
        }
    }
    outputFile.close();

    std::cout << BLUE_FG << "Size of bit array: " << sizeof(bitArray) / 1024 << "kb\n";
}

#endif //SEM2_1_H
