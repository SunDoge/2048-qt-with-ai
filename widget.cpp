#include "widget.h"
//#include "ui_widget.h"
#include <QApplication>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    ,
      // ui(new Ui::Widget),
      bPressFlag(false)
{
    // ui->setupUi(this);

    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor("#FAF8F0"));
    setPalette(pal);

    titleLabel = new QLabel(tr("2048"), this);
    titleLabel->setStyleSheet("color:#746D65");
    titleLabel->setFont(QFont("arial", 45, QFont::Black));

    tipsLabel =
            new QLabel(tr("Join the numbers and get to the 2048 tile!"), this);
    tipsLabel->setStyleSheet("color:#B3AFA7;");
    tipsLabel->setFont(QFont("arial", 10, QFont::Normal));

    bestScore = 0;

    QFile file("score.j");
    if (file.open(QIODevice::ReadOnly)) {
        file.read((char*)&bestScore, sizeof(bestScore));
        file.close();
    }

    gameWidget = new GameWidget(this);
    gameWidget->setFocus();
    connect(gameWidget, SIGNAL(ScoreInc(int)), this, SLOT(onScoreInc(int)));
    connect(gameWidget, SIGNAL(GameOver()), this, SLOT(onGameOver()));
    connect(gameWidget, SIGNAL(win()), this, SLOT(onWin()));

    QFont font;
    font.setFamily("Arial");
    font.setBold(true);
    font.setPixelSize(15);

    restartBtn = new QPushButton("New Game", this);
    //restartBtn->setGeometry(100, 120, 200, 50);
    restartBtn->setFont(font);
    restartBtn->setStyleSheet("QPushButton{color: white;background:#907963;border-style:flat;}");
    //restartBtn->setFlat(true);
    restartBtn->setFocusPolicy(Qt::NoFocus);
    connect(restartBtn, SIGNAL(clicked()), gameWidget, SLOT(restart()));

    bestScoreLabel = new QLabel(QString("BEST\n%1").arg(bestScore),this);
    bestScoreLabel->setFont(font);
    bestScoreLabel->setAlignment(Qt::AlignCenter);
    bestScoreLabel->setStyleSheet("QLabel{color:#FFFFEE;background:#BEAD9D}");

    scoreLabel = new QLabel("SCORE\n0", this);
    scoreLabel->setFont(font);
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet("QLabel{color:#FFFFEE;background:#BEAD9D}");

    closeBtn = new QPushButton("x", this);
    closeBtn->setFocusPolicy(Qt::NoFocus);
    closeBtn->setFont(QFont("Arial", 11, QFont::Normal));

    closeBtn->setStyleSheet("QPushButton{border-style:flat;color:#BDAD9F}"
                            "QPushButton:hover{border-style:flat;color:#FF0000}");
    closeBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->setGeometry(400 - 18, 3, 15, 15);
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(close()));

    resize(400, 510);
    //move((QApplication::desktop()->width() - width())/2,  (QApplication::desktop()->height() - height())/2);
}

Widget::~Widget()
{
    // delete ui;
    delete restartBtn;
    delete scoreLabel;
    delete bestScoreLabel;
    delete gameWidget;
}

void Widget::onScoreInc(int score)
{
    scoreLabel->setText(QString("Score:\n %1").arg(score));

    if (score > bestScore) {
        bestScore = score;
        bestScoreLabel->setText(QString("Score:\n %1").arg(bestScore));

        QFile file("score.j");
        file.open(QIODevice::WriteOnly);
        file.write((char*)&bestScore, sizeof(bestScore));
        file.close();
    }
}

void Widget::onGameOver()
{
    QMessageBox::information(this, "GameOver", "You lost !");
}

void Widget::onWin()
{
    QMessageBox::information(this, "Congratulation", "You win !");
}

void Widget::resizeEvent(QResizeEvent *)
{
    // 计算宽度和高度的缩放比例
    ratioW = width() / 400.0f;
    ratioH = height() / 510.0f;
    // 重置子部件大小和位置
    titleLabel->setGeometry(20 * ratioW, 40 * ratioH , 130 * ratioW, 50 * ratioH);
    tipsLabel->setGeometry(20 * ratioW, 100 * ratioH , 300 * ratioW, 20 * ratioH);
    gameWidget->setGeometry(18 * ratioW, 140 * ratioH, 365 * ratioW, 365 * ratioH);
    restartBtn->setGeometry(280 * ratioW, 90 * ratioH, 100 * ratioW, 30 * ratioH);
    bestScoreLabel->setGeometry(300 * ratioW, 40 * ratioH, 80 * ratioW, 40 * ratioH);
    scoreLabel->setGeometry(210 * ratioW, 40 * ratioH, 80 * ratioW, 40 * ratioH);
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    gameWidget->setFocus();
    QWidget::keyPressEvent(event);
}
void Widget::mousePressEvent ( QMouseEvent * event)
{
    bPressFlag = true;
    dragPosition = event->pos();
    QWidget::mousePressEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (bPressFlag) {
        QPoint relaPos(QCursor::pos() - dragPosition);
        move(relaPos);
    }
    QWidget::mouseMoveEvent(event);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    bPressFlag = false;
    QWidget::mouseReleaseEvent(event);
}
