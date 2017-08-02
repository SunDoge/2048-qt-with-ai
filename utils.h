#ifndef UTILS_H
#define UTILS_H

#include <vector>

using namespace std;

//const bool isAnimating;

enum GestureDirect {
    LEFT, RIGHT, UP, DOWN
};

struct Cell {
    int x;
    int y;
};

typedef Cell Vector;

struct Traversals {
    vector<int> x;
    vector<int> y;
};

class Tile {
public:
    Tile(Cell, int);
    int x;
    int y;
    int value;
    Cell previousPosition;
    Cell mergedFrom;

    void savePosition();
};

class Grid {
public:
    Grid();

    Grid(int array[4][4]);

    void setPlayerTurn(bool);

    vector<Cell> availableCells();

    double smoothness();

    double monotonicity2();

    double maxValue();

    bool move(int);

    Grid* clone();

    bool isWin();

    int islands();

    void mark(int x, int y);

    bool playerTurn;



    int cells[4][4];

    int visited[4][4];

    Vector vectors[4] = {
       { 0,  -1 }, // up
       { 1,  0 },  // right
       { 0,  1 },  // down
       { -1, 0 }   // left
    };

    Cell findFarthestPosition(Cell, Vector);

    bool withinBounds(Cell);

    bool cellOccupied(Cell);

    Traversals buildTraversals(Vector);

};

struct Result {
    int move;
    int score;
    int positions;
    int cutoffs;
};

class Utils
{
public:
    Utils();
};



#endif // UTILS_H
