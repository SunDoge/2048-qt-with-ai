#ifndef WIDGET_H
#define WIDGET_H

#include <QFile>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QWidget>
#include <QtGui>

//#include "ai.h"
#include "gamewidget.h"
//#include "ai.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget* parent = 0);
    ~Widget();

private:
    Ui::Widget* ui;

    QPushButton* restartBtn;
    QPushButton* closeBtn;
    QPushButton* aiBtn;

    QLabel* titleLabel;
    QLabel* tipsLabel;
    QLabel* scoreLabel;
    QLabel* bestScoreLabel;

    GameWidget* gameWidget;

    qreal ratioW, ratioH;

    int bestScore;
    QPoint dragPosition;
    bool bPressFlag;

protected:
    void resizeEvent(QResizeEvent*);
    void keyPressEvent(QKeyEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void closeEvent(QCloseEvent *e);

public slots:
    void onScoreInc(int);
    void onGameOver();
    void onWin();
    void onClose();
};

#endif // WIDGET_H
