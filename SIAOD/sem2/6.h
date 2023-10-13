//
// Created by sejapoe on 10/13/23.
//

#ifndef SEM2_6_H
#define SEM2_6_H

#include <vector>
#include <climits>
#include <iostream>
#include "colors.h"


void task6() {
    // init
    int n = 6;
    int **d = new int*[n];
    for (int i = 0; i < n * n; i++) {
        if (i % n == 0) d[i / n] = new int[n];
        d[i / n][i % n] = 99999999;
    }

    // add data
    d[0][1] = 8;
    d[0][2] = 4;
    d[1][3] = 6;
    d[1][4] = 3;
    d[2][1] = 3;
    d[2][3] = 2;
    d[2][5] = 10;
    d[3][4] = 3;
    d[3][5] = 1;
    d[4][5] = 4;

    // init path storing array
    int **next = new int*[n];
    for (int i = 0; i < n * n; i++) {
        if (i % n == 0) next[i / n] = new int[n];
        next[i / n][i % n] = d[i / 6][i % 6] != 99999999 ? i % 6 : -1;
    }

    // floyd algo
    for (int i = 0; i < n; i++) {
        for (int u = 0; u < n; u++) {
            for (int v = 0; v < n; v++) {
                if (d[u][i] + d[i][v] < d[u][v]) {
                    d[u][v] = d[u][i] + d[i][v];
                    next[u][v] = next[u][i];
                }
            }
        }
    }

    // print shortest path
    int from = 0, to = 5;
    if (d[from][to] == INT_MAX) {
        std::cout << RED_FG << "Path not found" << RESET << "\n";
        return;
    }
    while (from != to) {
        std::cout << (from + 1) << "->";
        from = next[from][to];
    }
    std::cout << (to + 1) << "\n";
}


#endif //SEM2_6_H
