#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include<QWidget>
#include<QMouseEvent>
#include<QEventLoop>
#include<QTimer>
#include<QPainter>
#include<QList>
#include<vector>

#include "utils.h"
//#include "ai.h"

using namespace std;

enum AnimationType {
    MOVE, APPEARANCE
};

struct Animation {
    AnimationType type;
    GestureDirect direct;
    QPointF startPos;
    QPointF endPos;
    int digit0;
    int digit1;
};

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = 0);

    // For AI to eval
    bool playerTurn();
    vector<Cell> availableCells();
    double smoothness();
    double monotonicity2();
    double maxValue();
    GameWidget* clone();
    void setBoard(int b[4][4]);

    int board[4][4];

private:


    int digitCount;
    int score;

    QPoint startPos;
    QList<Animation> animationList;

    qreal w, h;

    QImage *cacheImg;

    bool isAnimating;

    bool checkGameOver();

    bool checkWin();

    int getBitCount(int);

    bool playAnimation(Animation &, QPainter &);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

//    AI ai;
    Cell findFarthestPosition(Cell, Vector);
    bool withinBounds(Cell);
    bool cellOccupied(Cell);

    Vector vectors[4] = {
       { 0,  -1 }, // up
       { 1,  0 },  // right
       { 0,  1 },  // down
       { -1, 0 }   // left
    };

signals:
    void GestureMove(GestureDirect);

    void ScoreInc(int);

    void GameOver();

    void Win();

public slots:
    void onGestureMove(GestureDirect);
    void restart();
    void autorun();
};

#endif // GAMEWIDGET_H
