//
// Created by Sun Doge on 2017/8/2.
//

#ifndef AI_H
#define AI_H

#include <ctime>
#include <QtCore>

#include "grid.h"

struct Result {
    int move;
    double score;
    int positions;
    int cutoffs;
};

//time_t minSearchTime = 1;


class AI {
public:
    AI(Grid *g);

    int getBest();

private:
    Grid *grid;

    double eval();

    Result search(int depth, double alpha, double beta, int positions, int cutoffs);

    Result iterativeDeep();
};


#endif //AI_H



