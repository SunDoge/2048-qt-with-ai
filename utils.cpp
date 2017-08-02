#include<cmath>

#include "utils.h"

// Tile ========================================
Tile::Tile(Cell position, int v)
{
    x = position.x;
    y = position.y;
    value = v;
}

void Tile::savePosition()
{
    previousPosition = {
        x, y
    };
}


// Grid ========================================

Grid::Grid() {

}

Grid::Grid(int array[4][4]) {
    for(int i=0; i<4; i++) {
        for(int j = 0; j < 4; j++) {
            cells[i][j] = array[i][j];
        }
    }
}

void Grid::setPlayerTurn(bool t)
{
    playerTurn = t;
}

vector<Cell> Grid::availableCells() {
    vector<Cell> c;
    for(int i = 0; i < 4; i++) {
        for (int j = 0; j< 4; j++) {
            if(!cells[i][j]) {
                c.push_back({i,j});
            }
        }
    }
    return c;
}

double Grid::smoothness() {
    double smoothness = 0;
    for(int x=0; x<4; x++) {
        for(int y = 0; y<4 ; y++) {
            if(cells[x][y]) {
                double value = log(cells[x][y]) / log(2);
                for (int direction = 1; direction <=2; direction++) {
                    Vector vector = vectors[direction];

                    Cell targetCell = findFarthestPosition({x,y}, vector);

                    if(cellOccupied(targetCell)) {
                        int target = cells[targetCell.x][targetCell.y];
                        double targetValue = log(target) / log(2);
                        smoothness -= abs(value - targetValue);
                    }
                }
            }
        }
    }

    return smoothness;
}

double Grid::monotonicity2()
{
    double totals[4] = {0,0,0,0};
    for(int x = 0; x < 4; x++) {
        int current = 0;
        int next = current + 1;
        while(next < 4) {
            while(next < 4 && !cellOccupied({x, next})) {
                next++;
            }
            if(next >= 4) {
                next--;
            }
            double currentValue = cellOccupied({x, current}) ?
                    log(cells[x][current]) / log(2) :
                    0;
            double nextValue = cellOccupied({x, next}) ?
                    log(cells[x][next]) / log(2) :
                    0;
          if (currentValue > nextValue) {
            totals[0] += nextValue - currentValue;
          } else if (nextValue > currentValue) {
            totals[1] += currentValue - nextValue;
          }
          current = next;
          next++;
        }
    }

    for(int y = 0; y < 4; y++) {
        int current = 0;
        int next = current + 1;
        while(next < 4) {
            while(next < 4 && !cellOccupied({next, y})) {
                next++;
            }
            if(next >= 4) {
                next--;
            }
            double currentValue = cellOccupied({current, y}) ?
                    log(cells[current][y]) / log(2) :
                    0;
            double nextValue = cellOccupied({next, y}) ?
                    log(cells[next][y]) / log(2) :
                    0;
          if (currentValue > nextValue) {
            totals[2] += nextValue - currentValue;
          } else if (nextValue > currentValue) {
            totals[3] += currentValue - nextValue;
          }
          current = next;
          next++;
        }
    }

    return max(totals[0], totals[1]) + max(totals[2], totals[3]);
}

double Grid::maxValue()
{
    int max = 0;
      for (int x=0; x<4; x++) {
        for (int y=0; y<4; y++) {
            if (cellOccupied({x, y})) {
            int value = cells[x][y];
            if (value > max) {
              max = value;
            }
          }
        }
      }

      return log(max) / log(2);
}

bool Grid::move(int direction)
{
    // 0: up, 1: right, 2:down, 3: left
    Grid *self = this;
    Cell cell;
    int tile;
    Vector vector = vectors[direction];
    Traversals traversals = buildTraversals(vector);
    bool moved = false;
//    int score = 0;
//    bool won = false;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            int x = traversals.x[i];
            int y = traversals.y[j];
            cell = {x, y};
            if(cells[x][y]) {
                Cell next = findFarthestPosition(cell, vector);

                if(cells[next.x][next.y] == cells[x][y]) {
                    cells[next.x][next.y] += cells[x][y];
                }

                if(!(next.x == x && next.y == y)) {
                    playerTurn = false;
                    moved = true;
                }
            }
        }
    }

    return moved;

}

Grid* Grid::clone()
{
//    Grid* newGrid(cells);
//    return newGrid;
}

bool Grid::isWin()
{
    for(int x = 0; x < 4; x++) {
        for(int y = 0; y < 4; y++) {
            if(cells[x][y] == 2048) {
                return true;
            }
        }
    }
    return false;
}

int Grid::islands()
{
    for(int x = 0; x < 4; x++) {
        for(int y = 0; y < 4; y++) {
            visited[x][y] = 0;
        }
    }

    int islands = 0;
    for(int x = 0; x < 4; x++) {
        for(int y = 0; y < 4; y++) {
            if(cells[x][y] && !visited[x][y]) {
                islands++;
                mark(x, y);
            }
        }
    }
}

void Grid::mark(int x, int y)
{
    if(x >= 0 && x < 4 && y >=0 && y < 4 && !visited[x][y] && cells[x][y]) {
        visited[x][y] = 1;

        mark(x - 1, y);
        mark(x, y + 1);
        mark(x + 1, y);
        mark(x, y - 1);
    }
}

Traversals Grid::buildTraversals(Vector vector)
{
    Traversals traversals;
    for(int pos = 0; pos < 4; pos++) {
        traversals.x.push_back(pos);
        traversals.y.push_back(pos);
    }

    if(vector.x == 1) reverse(traversals.x.begin(), traversals.x.end());
    if(vector.y == 1) reverse(traversals.y.begin(), traversals.y.end());

    return traversals;
}

Cell Grid::findFarthestPosition(Cell cell, Vector vector)
{
    Cell previous;
    do {
        previous = cell;
        cell = {
            previous.x + vector.x, previous.y + vector.y
        };
    }while (withinBounds(cell) && !cellOccupied(cell));

    return cell;
}

bool Grid::withinBounds(Cell position)
{
    return position.x >= 0 && position.x < 4 &&
             position.y >= 0 && position.y < 4;
}

bool Grid::cellOccupied(Cell cell)
{
    return cells[cell.x][cell.y];
}

// AI ================================================

Utils::Utils()
{

}
