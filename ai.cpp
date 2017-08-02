#include <vector>
#include <cmath>

#include "ai.h"
#include "gamewidget.h"

using namespace std;

//Grid::Grid(int array[4][4]) {
//    for(int i=0; i<4; i++) {
//        for(int j = 0; j < 4; j++) {
//            cells[i][j] = array[i][j];
//        }
//    }
//}

//vector<Cell> Grid::availableCells() {
//    vector<Cell> c;
//    for(int i = 0; i < 4; i++) {
//        for (int j = 0; j< 4; j++) {
//            if(!cells[i][j]) {
//                c.push_back({i,j});
//            }
//        }
//    }
//    return c;
//}

//double Grid::smoothness() {
//    double smoothness = 0;
//    for(int x=0; x<4; x++) {
//        for(int y = 0; y<4 ; y++) {
//            if(cells[x][y]) {
//                double value = log(cells[x][y]) / log(2);
//                //
//            }
//        }
//    }
//}

AI::AI() {

}

AI::AI(Grid *grid) {
    this->grid = grid;
}

//AI::AI(int array[4][4])
//{
//    Grid grid(array);
//}

AI::AI(GameWidget *gameWidget) {
//    grid = gameWidget;
    grid = new Grid(gameWidget->board);
}

double AI::eval()
{
    int emptyCells = grid->availableCells().size();

    double smoothWeight = 0.1,
            //monoWeight   = 0.0,
            //islandWeight = 0.0,
            mono2Weight  = 1.0,
            emptyWeight  = 2.7,
            maxWeight    = 1.0;

    return grid->smoothness() * smoothWeight
           //+ this.grid.monotonicity() * monoWeight
           //- this.grid.islands() * islandWeight
           + grid->monotonicity2() * mono2Weight
           + log(emptyCells) * emptyWeight
           + grid->maxValue() * maxWeight;

}

Result AI::search(int depth, double alpha, double beta, int positions, int cutoffs)
{
//    double bestScore = 0;
//    int bestMove = -1;
//    Result result;

//    if (!grid->playerTurn) {
//        bestScore = alpha;

//        for(int direction = 0; direction < 4; direction++) {
//            Grid* newGrid = grid->clone();
//            if(newGrid->move(direction)) {
//                positions++;
//                if (newGrid->isWin()) {
//                    return { direction, 10000, positions, cutoffs };
//                }

//                AI newAI(newGrid);
//                if(depth == 0) {
////                    result = {direction, newAI.eval(), positions, cutoffs };
//                } else {
//                    result = newAI.search(depth-1, bestScore, beta, positions, cutoffs);
//                    if(result.score > 9900) {
//                        result.score--;
//                    }
//                    positions = result.positions;
//                    cutoffs = result.cutoffs;

//                }

//                if(result.score > bestScore) {
//                    bestScore = result.score;
//                    bestMove = direction;
//                }

//                if(bestScore > beta) {
//                    cutoffs++;
////                    return {
////                        bestMove, beta, positions, cutoffs
////                    };
//                }
//            }
//        }
//    } else {
//        bestScore = beta;

//        vector<int> candidates;

//        vector<Cell> cells = grid->availableCells();

//        vector<double> scores2, scores4;

////        for(int value = 2; value <=4; value*= 2) {
//            for(int i = 0; i < cells.size(); i++) {
//                Cell cell = cells[i];
//                grid->cells[cell.x][cell.y] = 2;
//                scores2.push_back(-grid->smoothness() + grid->islands());
//                grid->cells[cell.x][cell.y] = 4;
//                scores4.push_back(-grid->smoothness() + grid->islands());
//            }
////        }

//         vector<double>::iterator max2 = max_element(scores2.begin(), scores2.end());
//         int max2p = distance(scores2.begin(), max2);
//         vector<double>::iterator max4 = max_element(scores4.begin(), scores4.end());
//         int max4p = distance(scores4.begin(), max4);
//    }


}


