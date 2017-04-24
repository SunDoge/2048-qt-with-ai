#ifndef UTILS_H
#define UTILS_H

#include <vector>

using namespace std;

enum GestureDirect {
    LEFT, RIGHT, UP, DOWN
};

struct Cell {
    int x;
    int y;
};

class Grid {
public:
    Grid();
    Grid(int array[4][4]);

private:
    int cells[4][4];
    vector<Cell> availableCells();
    double smoothness();
    double monotonicity2();
    double maxValue();

};

struct Result {
    GestureDirect move;
    int score;
    Cell positions;
    int cutoffs;
};

class Utils
{
public:
    Utils();
};

#endif // UTILS_H
