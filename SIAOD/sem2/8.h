//
// Created by sejapoe on 10/13/23.
//

#ifndef SEM2_8_H
#define SEM2_8_H

#include <iostream>

void task8() {
    // ради удобства, ввод захардкожен (шоб 25 чисел ручками не вводить)
    int n = 5, m = 5;
    int **data = new int *[n]{
            new int[m]{4, 16, 4, 23, 1},
            new int[m]{12, 10, 11, 13, 2},
            new int[m]{6, 17, 8, 4, 5},
            new int[m]{9, 10, 1, 2, 3},
            new int[m]{4, 4, 5, 6, 7},
    };
    int **dp = new int *[n];
    for (int i = 0; i < n; ++i) {
        dp[i] = new int[m];
    }

    dp[0][0] = data[0][0];
    for (int i = 1; i < n; ++i) {
        dp[i][0] = dp[i - 1][0] + data[i][0];
    }
    for (int i = 1; i < m; ++i) {
        dp[0][i] = dp[0][i - 1] + data[0][i];
    }

    for (int i = 1; i < n; i++) {
        for (int j = 1; j < n; j++) {
            dp[i][j] = std::min(std::min(dp[i][j - 1], dp[i - 1][j]), dp[i - 1][j - 1]) + data[i][j];
        }
    }
    std::cout << dp[n - 1][m - 1] << "\n";
}

#endif //SEM2_8_H
