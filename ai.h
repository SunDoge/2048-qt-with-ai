#ifndef AI_H
#define AI_H

#include <vector>
#include "utils.h"

using namespace std;



//class Grid {
//public:
//    Grid();
//    Grid(int array[4][4]);
    
//private:
//    int cells[4][4];
//    vector<Cell> availableCells();
//    double smoothness();
//    double monotonicity2();
//    double maxValue();
    
//};

class AI
{
public:
    AI();
    AI(int array[4][4]);

private:
    //int grid[4][4];
    Grid grid;
    Result search(int, double, double, Cell);
    Result iterativeDeep();
    Result getBest();
};

#endif // AI_H
