#include <vector>
#include <cmath>

#include "ai.h"

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

AI::AI(int array[4][4])
{
    Grid grid(array);
}


