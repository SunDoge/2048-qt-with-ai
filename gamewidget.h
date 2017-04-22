#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include<QWidget>
#include<QMouseEvent>
#include<QEventLoop>
#include<QTimer>
#include<QPainter>
#include<QList>

#include "ai.h"

enum GestureDirect {
    LEFT, RIGHT, UP, DOWN
};

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

private:
    int board[4][4];

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

    AI ai;

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
