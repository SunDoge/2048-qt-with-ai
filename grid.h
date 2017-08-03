//
// Created by Sun Doge on 2017/8/2.
//

#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

struct Cell {
    int x;
    int y;
    int value;
};

class Grid {
public:

    // To make them access to ai
    bool playerTurn;

    int cells[4][4];

    int oldCells[4][4];

    int visited[4][4];

    Grid(int a[4][4]);

    ~Grid();

    bool move(int direction);

    bool isWin();

    int smoothness();

    int monotonicity2();

    int maxValue();

    int islands();

    void mark(int x, int y, int value);

    vector<Cell> availableCells();

    Grid *clone();

    void update(int a[4][4]);
};


#endif //GRID_H



