#include "gamewidget.h"
#include<QDebug>
//#include<cmath>
//#include<vector>

using namespace std;

QColor digitBkg[11] = {QColor::fromRgb(0xEE, 0xE5, 0xDB), QColor::fromRgb(0xEC, 0xE0, 0xC8),
                       QColor::fromRgb(0xF2, 0xAF, 0x78), QColor::fromRgb(0xEE, 0x8A, 0x54),
                       QColor::fromRgb(0xFE, 0x76, 0x5E), QColor::fromRgb(0xE7, 0x58, 0xC),
                       QColor::fromRgb(0xFF, 0x66, 0x66), QColor::fromRgb(0xF1, 0xCF, 0x48),
                       QColor::fromRgb(0xCC, 0x33, 0x33), QColor::fromRgb(0xE3, 0xB9, 0x11),
                       QColor::fromRgb(0xFF, 0x00, 0x00)};

QPointF dPos[5] = {QPointF(-10, 0), QPointF(10, 0), QPointF(0, -10), QPointF(0, 10), QPointF(-2, -2)};




GameWidget::GameWidget(QWidget *parent) : QWidget(parent)
{
    connect(this, SIGNAL(GestureMove(GestureDirect)), SLOT(onGestureMove(GestureDirect)));
    memset(board, 0, sizeof(board));
    srand(time(NULL));
    board[rand() % 4][rand() %4] = 2;
    while(true) {
        int x = rand() % 4;
        int y = rand() % 4;
        if (board[x][y] != 0) {
            continue;
        }
        else {
            board[x][y] = 2;
            break;
        }
    }
    score = 0;

    digitCount = 2;
    isAnimating = false;
    isAutorunning = false;
    grid = new Grid(board);
    ai = new AI(grid);
    cacheImg = NULL;
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if (isAnimating)
        return;
    switch (event->key()) {
    case Qt::Key_Left:
        emit GestureMove(LEFT);
        break;
    case Qt::Key_Right:
        emit GestureMove(RIGHT);
        break;
    case Qt::Key_Down:
        emit GestureMove(DOWN);
        break;
    case Qt::Key_Up:
        emit GestureMove(UP);
        break;
    default:
        break;
    }
    QWidget::keyPressEvent(event);
}

void GameWidget::mousePressEvent(QMouseEvent *e)
{
    // 获取起点坐标
    startPos = e->pos();
}

void GameWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // 如果在播放动画效果则直接退出防止重复产生手势事件
    if (isAnimating)
        return;
    // 根据终点坐标和起点坐标计算XY坐标的增量
    float dX = (float)(e->pos().x() - startPos.x());
    float dY = (float)(e->pos().y() - startPos.y());
    // 确定手势方向
    if (std::abs(dX) > std::abs(dY))
    {
        if (dX < 0)
            emit GestureMove(LEFT);
        else
            emit GestureMove(RIGHT);
    }
    else
    {
        if (dY < 0)
            emit GestureMove(UP);
        else
            emit GestureMove(DOWN);
    }
}

void GameWidget::onGestureMove(GestureDirect direct)
{
    int i, j, k;
    Animation a;
    // 是否合并过方格
    // bool combine = false;
    // 处理不同方向
    switch (direct)
    {
    // 向左
    case LEFT:
        // 循环每一行
        for (i = 0; i < 4; i++)
        {
            /* 初始化j k为0
             * 这里j表示要交换的数字列号
             * k表示交换到的位置的列号
             * */
            j = 0, k = 0;
            while (true)
            {
                // 循环找到第一个不是0的数字对应的列号
                while (j < 4 && board[i][j] == 0)
                    j++;
                // 如果超过了3则说明搜索完毕 推出循环
                if (j > 3)
                    break;

                // 交换两个数字
                qSwap(board[i][k], board[i][j]);
                // 记录动画信息
                a.type = MOVE;
                a.startPos = QPointF(7 + (w + 5) * j, 7 + (h + 5) * i);
                a.endPos = QPointF(7 + (w + 5) * k, 7 + (h + 5) * i);
                a.digit0 = a.digit1 = board[i][k];
                a.direct = LEFT;
                //如果交换后的数字与其前一列的数字相同
                if (k > 0 && board[i][k] == board[i][k - 1])
                {
                    // 前一列的数字*2
                    board[i][k - 1] <<= 1;
                    // 这一列的数字置为0
                    board[i][k] = 0;
                    // 记录动画信息
                    a.digit1 = board[i][k - 1];
                    a.endPos = QPointF(7 + (w + 5) * (k - 1), 7 + (h + 5) * i);
                    // 增加分数
                    score += board[i][k - 1];
                    // 发射增加分数的信号
                    emit ScoreInc(score);
                    // 数码个数-1
                    digitCount--;
//                    combine = true;
                }
                else
                    k++;
                j++;
                // 添加到动画链表
                animationList.append(a);
            }
        }
        break;
        // 其余三个方向与左向类似
    case RIGHT:
        for (i = 0; i < 4; i++)
        {
            j = 3, k = 3;
            while (true)
            {
                while (j > -1 && board[i][j] == 0)
                    j--;
                if (j < 0)
                    break;
                qSwap(board[i][k], board[i][j]);
                a.type = MOVE;
                a.startPos = QPointF(7 + (w + 5) * j, 7 + (h + 5) * i);
                a.endPos = QPointF(7 + (w + 5) * k, 7 + (h + 5) * i);
                a.digit0 = a.digit1 = board[i][k];
                a.direct = RIGHT;
                if (k < 3 && board[i][k] == board[i][k + 1])
                {
                    board[i][k + 1] <<= 1;
                    board[i][k] = 0;
                    a.digit1 = board[i][k + 1];
                    a.endPos = QPointF(7 + (w + 5) * (k + 1), 7 + (h + 5) * i);
                    score += board[i][k + 1];
                    emit ScoreInc(score);
                    digitCount--;
//                    combine = true;
                }
                else
                    k--;
                j--;
                animationList.append(a);
            }
        }
        break;
    case UP:
        for (i = 0; i < 4; i++)
        {
            j = 0, k = 0;
            while (true)
            {
                while (j < 4 && board[j][i] == 0)
                    j++;
                if (j > 3)
                    break;
                qSwap(board[k][i], board[j][i]);
                a.type = MOVE;
                a.startPos = QPointF(7 + (w + 5) * i, 7 + (h + 5) * j);
                a.endPos = QPointF(7 + (w + 5) * i, 7 + (h + 5) * k);
                a.digit0 = a.digit1 = board[k][i];
                a.direct = UP;
                if (k > 0 && board[k][i] == board[k - 1][i])
                {
                    board[k - 1][i] <<= 1;
                    board[k][i] = 0;
                    a.digit1 = board[k - 1][i];
                    a.endPos = QPointF(7 + (w + 5) * i, 7 + (h + 5) * (k - 1));
                    score += board[k - 1][i];
                    emit ScoreInc(score);
                    digitCount--;
//                    combine = true;
                }
                else
                    k++;
                j++;
                animationList.append(a);
            }
        }
        break;
    case DOWN:
        for (i = 0; i < 4; i++)
        {
            j = 3, k = 3;
            while (true)
            {
                while (j > -1 && board[j][i] == 0)
                    j--;
                if (j < 0)
                    break;
                qSwap(board[k][i], board[j][i]);
                a.type = MOVE;
                a.startPos = QPointF(7 + (w + 5) * i, 7 + (h + 5) * j);
                a.endPos = QPointF(7 + (w + 5) * i, 7 + (h + 5) * k);
                a.digit0 = a.digit1 = board[k][i];
                a.direct = DOWN;
                if (k < 3 && board[k][i] == board[k + 1][i])
                {
                    board[k + 1][i] <<= 1;
                    board[k][i] = 0;
                    a.digit1 = board[k + 1][i];
                    a.endPos = QPointF(7 + (w + 5) * i, 7 + (h + 5) * (k + 1));
                    score += board[k + 1][i];
                    emit ScoreInc(score);
                    digitCount--;
//                    combine = true;
                }
                else
                    k--;
                j--;
                animationList.append(a);
            }
        }
        break;
    }
    bool flag_move = false;
    for(int index = 0; index < animationList.size(); index++) {
        if (animationList.at(index).startPos != animationList.at(index).endPos) {
            flag_move = true;
            break;
        }
    }
    // 如果数字木有填满
    if (digitCount != 16 && flag_move)
    {
        // 随机产生行号和列号
        i = rand() % 4, j = rand() % 4;
        // 循环直到行和列对应的元素为0
        while (board[i][j] != 0)
            i = rand() % 4, j = rand() % 4;
        // 填入2
//        board[i][j] = (rand() % 2 + 1) * 2;
//        board[i][j] = 2;
        board[i][j] = rand() / double(RAND_MAX) < 0.9 ? 2 : 4;
        // 记录动画信息
        a.type = APPEARANCE;
        a.startPos = a.endPos = QPointF(7 + (w + 5) * j, 7 + (h + 5) * i);
        a.startPos += QPointF(w / 2, h / 2);
        a.digit0 = board[i][j];
        // 数码个数加一
        digitCount++;
    }
    else
    {
        // 如果数字填满了 检测游戏是否over
        if (checkGameOver()) {
            isAutorunning = false;
            emit GameOver();// 如果游戏over了则发射GameOver信号
        }
    }

    // 开始绘制动画效果
    isAnimating = true;
    // 动画列表的迭代器
    QList<Animation>::iterator it;
    // 事件循环 用于延时
    QEventLoop eventLoop;
    // 删除之前的缓存图像
    if (cacheImg)
        delete cacheImg;
    // 建立缓存图像
    cacheImg = new QImage(width(), height(), QImage::Format_ARGB32);
    // 清空图像
    cacheImg->fill(0);
    // 构造一个QPainter对象
    QPainter painter(cacheImg);
    // 字体
    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(35);
    painter.setFont(font);
    // 标识所有方格动画是否都播放完毕
    bool ok = false;
    while (true)
    {
        // 构造一个画刷 颜色为R G B分量分别为141 121 81的颜色
        QBrush brush(QColor::fromRgb(141, 121, 81));
        // 使painter应用这个画刷
        painter.setBrush(brush);

        // 设置画笔为空笔 目的是使绘制的图形没有描边
        painter.setPen(Qt::NoPen);

        // 绘制一个矩形
        painter.drawRect(2, 2, width() - 4, height() - 4);

        // 设置画刷颜色为 RGB分量为171 165 141的颜色
        brush.setColor(QColor::fromRgb(171, 165, 141));
        // 应用这个画刷
        painter.setBrush(brush);

        // 循环绘制游戏面板
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                // 绘制小方格
                painter.drawRect(QRectF(7 + (w + 5) * j, 7 + (h + 5) * i, w, h));

        // 假设都播放完毕
        ok = true;

        // 循环播放每个方格动画
        for (it = animationList.begin(); it != animationList.end(); it++)
            if (!playAnimation(*it, painter))
                ok = false;

        // 刷新部件
        update();

        // 全部播放完则退出
        if (ok)
            break;

        // 延时5ms
        QTimer::singleShot(1, &eventLoop, SLOT(quit()));
        eventLoop.exec();
    }
    // 播放方格出现的动画
    while (!playAnimation(a, painter))
    {
        update();
        QTimer::singleShot(1, &eventLoop, SLOT(quit()));
        eventLoop.exec();
    }
    //清除所有动画
    animationList.clear();
    //刷新当前部件
    isAnimating = false;

    // 检测游戏是否获胜
    if (checkWin())
        emit Win();// 如果获胜则发射win信号

    update();
}

bool GameWidget::playAnimation(Animation& a, QPainter& painter)
{
    bool rtn = false;
    QBrush brush(Qt::SolidPattern);

    // 移动方格位置
    if (a.type == MOVE)
    {
        switch (a.direct)
        {
        case LEFT:
            if (a.startPos.x() > a.endPos.x())
                a.startPos += dPos[LEFT];
            else
                a.startPos = a.endPos, rtn = true;
            break;
        case RIGHT:
            if (a.startPos.x() < a.endPos.x())
                a.startPos += dPos[RIGHT];
            else
                a.startPos = a.endPos, rtn = true;
            break;
        case UP:
            if (a.startPos.y() > a.endPos.y())
                a.startPos += dPos[UP];
            else
                a.startPos = a.endPos, rtn = true;
            break;
        case DOWN:
            if (a.startPos.y() < a.endPos.y())
                a.startPos += dPos[DOWN];
            else
                a.startPos = a.endPos, rtn = true;
        }
        // 如果方格移动到终点
        if (!rtn)
        {
            brush.setColor(digitBkg[getBitCount(a.digit0)]);
            painter.setBrush(brush);
            painter.drawRect(QRectF(a.startPos.x(), a.startPos.y(), w, h));
            painter.setPen(QColor::fromRgb(0, 0, 0));
            painter.drawText(QRectF(a.startPos.x(), a.startPos.y(), w, h), Qt::AlignCenter,
                             QString::number(a.digit0));
        }
        else
        {
            brush.setColor(digitBkg[getBitCount(a.digit1)]);
            painter.setBrush(brush);
            painter.drawRect(QRectF(a.startPos.x(), a.startPos.y(), w, h));
            painter.setPen(QColor::fromRgb(0, 0, 0));
            painter.drawText(QRectF(a.startPos.x(), a.startPos.y(), w, h), Qt::AlignCenter,
                             QString::number(a.digit1));
        }
        painter.setPen(Qt::NoPen);
    }
    else
    {
        // 方格出现的动画效果
        if (a.startPos.x() > a.endPos.x())
            a.startPos += dPos[4];
        else
            a.startPos = a.endPos, rtn = true;
        brush.setColor(digitBkg[getBitCount(a.digit0)]);
        painter.setBrush(brush);
        painter.drawRect(QRectF(a.startPos.x(), a.startPos.y(),
                                w - 2 * (a.startPos.x() - a.endPos.x()),
                                h - 2 * (a.startPos.y() - a.endPos.y())));
        painter.setPen(QColor::fromRgb(0, 0, 0));
        painter.drawText(QRectF(a.endPos.x(), a.endPos.y(), w, h),
                         Qt::AlignCenter, QString::number(a.digit0));
        painter.setPen(Qt::NoPen);
    }
    return rtn;
}

void GameWidget::paintEvent(QPaintEvent *)
{
    // 构造一个QPainter对象 使用它来进行绘图
    QPainter painter(this);

    // 如果正在播放动画效果则绘制缓存位图
    if (isAnimating)
    {
        painter.drawImage(0, 0, *cacheImg);
        return;
    }

    // 构造一个画刷 颜色为R G B分量分别为141 121 81的颜色
    QBrush brush(QColor::fromRgb(141, 121, 81));
    // 使painter应用这个画刷
    painter.setBrush(brush);

    // 设置画笔为空笔 目的是使绘制的图形没有描边
    painter.setPen(Qt::NoPen);

    // 绘制一个矩形
    painter.drawRect(2, 2, width() - 4, height() - 4);

    // 计算每个小格子的宽度和高度
    w = width() - 4, h = height() - 4;
    w = (w - 25) / 4, h = (h - 25) / 4;

    /* 构造一个字体
     * 字体名字为Consolas
     * 字体设置为粗体
     * 字体大小为40像素
     * */
    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(35);
    // 使painter应用这个字体
    painter.setFont(font);

    // 循环绘制游戏面板
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            // 如果方格中有数字
            if (board[i][j])
            {
                // 设置画刷颜色为数码对应的颜色
                brush.setColor(digitBkg[getBitCount(board[i][j])]);
                // 应用这个画刷
                painter.setBrush(brush);
                // 绘制一个小方格
                painter.drawRect(QRectF(7 + (w + 5) * j, 7 + (h + 5) * i, w, h));
                // 设置画笔为黑色画笔
                painter.setPen(QColor::fromRgb(0, 0, 0));
                // 绘制数码
                painter.drawText(QRectF(7 + (w + 5) * j, 7 + (h + 5) * i, w, h), Qt::AlignCenter,
                                 QString::number(board[i][j]));
                // 设置画笔为空笔
                painter.setPen(Qt::NoPen);
            }
            // 如果方格中没有数字
            else
            {
                // 设置画刷颜色为 RGB分量为171 165 141的颜色
                brush.setColor(QColor::fromRgb(171, 165, 141));
                // 应用这个画刷
                painter.setBrush(brush);
                // 绘制小方格
                painter.drawRect(QRectF(7 + (w + 5) * j, 7 + (h + 5) * i, w, h));
            }
        }
}

void GameWidget::restart()
{
    // 初始化相关变量 同构造函数
    score = 0;
    digitCount = 2;
    memset(board, 0, sizeof(board));
    board[rand() % 4][rand() % 4] = 2;
    while(true) {
        int x = rand() % 4;
        int y = rand() % 4;
        if (board[x][y] != 0) {
            continue;
        }
        else {
            board[x][y] = 2;
            break;
        }
    }
    emit ScoreInc(score);
    update();
}

bool GameWidget::checkGameOver()
{
    // 循环检测是否含有相邻的相同数码
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if (j != 3 && board[i][j] == board[i][j + 1])
                return false;
            if (i != 3 && board[i][j] == board[i + 1][j])
                return false;
        }
    return true;
}

bool GameWidget::checkWin()
{
    // 循环检测是否某个方格的数字为2048
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (board[i][j] == 2048) {
                isAutorunning = false;
                return true;
            }


    return false;
}

int GameWidget::getBitCount(int n)
{
    // 循环获取数字二进制位数
    int c = 0;
    while (n >>= 1)
        c++;
    // 返回位数-1
    return c - 1;
}

// ******
// For AI
// ******

void GameWidget::autorun() {

    isAutorunning = !isAutorunning;

    while (isAutorunning && !isAnimating) {

//    if(!isAnimating) {

        grid->update(board);

        int direction = ai->getBest();

//        cout << "AI says " << direction << endl;
        switch (direction) {
        case 0:
            emit GestureMove(LEFT);
            break;

        case 1:
            emit GestureMove(RIGHT);
            break;

        case 2:
            emit GestureMove(UP);
            break;

        case 3:
            emit GestureMove(DOWN);
            break;
        default:
            break;
        }
    }

}

//bool GameWidget::playerTurn() {
//    return !isAnimating;
//}

//vector<Cell> GameWidget::availableCells() {
//    vector<Cell> c;
//    for(int i = 0; i < 4; i++) {
//        for (int j = 0; j< 4; j++) {
//            if(!board[i][j]) {
//                c.push_back({i,j});
//            }
//        }
//    }
//    return c;
//}

//double GameWidget::smoothness() {
//    double smoothness = 0;
//    for(int x=0; x<4; x++) {
//        for(int y = 0; y<4 ; y++) {
//            if(board[x][y]) {
//                double value = log(board[x][y]) / log(2);
//                for (int direction = 1; direction <=2; direction++) {
//                    Vector vector = vectors[direction];

//                    Cell targetCell = findFarthestPosition({x,y}, vector);

//                    if(cellOccupied(targetCell)) {
//                        int target = board[targetCell.x][targetCell.y];
//                        double targetValue = log(target) / log(2);
//                        smoothness -= abs(value - targetValue);
//                    }
//                }
//            }
//        }
//    }
//    return smoothness;
//}

//double GameWidget::monotonicity2()
//{
//    double totals[4] = {0,0,0,0};
//    for(int x = 0; x < 4; x++) {
//        int current = 0;
//        int next = current + 1;
//        while(next < 4) {
//            while(next < 4 && !cellOccupied({x, next})) {
//                next++;
//            }
//            if(next >= 4) {
//                next--;
//            }
//            double currentValue = cellOccupied({x, current}) ?
//                    log(board[x][current]) / log(2) :
//                    0;
//            double nextValue = cellOccupied({x, next}) ?
//                    log(board[x][next]) / log(2) :
//                    0;
//          if (currentValue > nextValue) {
//            totals[0] += nextValue - currentValue;
//          } else if (nextValue > currentValue) {
//            totals[1] += currentValue - nextValue;
//          }
//          current = next;
//          next++;
//        }
//    }

//    for(int y = 0; y < 4; y++) {
//        int current = 0;
//        int next = current + 1;
//        while(next < 4) {
//            while(next < 4 && !cellOccupied({next, y})) {
//                next++;
//            }
//            if(next >= 4) {
//                next--;
//            }
//            double currentValue = cellOccupied({current, y}) ?
//                    log(board[current][y]) / log(2) :
//                    0;
//            double nextValue = cellOccupied({next, y}) ?
//                    log(board[next][y]) / log(2) :
//                    0;
//          if (currentValue > nextValue) {
//            totals[2] += nextValue - currentValue;
//          } else if (nextValue > currentValue) {
//            totals[3] += currentValue - nextValue;
//          }
//          current = next;
//          next++;
//        }
//    }

//    return max(totals[0], totals[1]) + max(totals[2], totals[3]);
//}

//double GameWidget::maxValue()
//{
//    int max = 0;
//      for (int x=0; x<4; x++) {
//        for (int y=0; y<4; y++) {
//            if (cellOccupied({x, y})) {
//            int value = board[x][y];
//            if (value > max) {
//              max = value;
//            }
//          }
//        }
//      }

//      return log(max) / log(2);
//}

//Cell GameWidget::findFarthestPosition(Cell cell, Vector vector)
//{
//    Cell previous;
//    do {
//        previous = cell;
//        cell = {
//            previous.x + vector.x, previous.y + vector.y
//        };
//    }while (withinBounds(cell) && !cellOccupied(cell));

//    return cell;
//}

//bool GameWidget::withinBounds(Cell position)
//{
//    return position.x >= 0 && position.x < 4 &&
//             position.y >= 0 && position.y < 4;
//}

//bool GameWidget::cellOccupied(Cell cell)
//{
//    return board[cell.x][cell.y];
//}

//GameWidget* GameWidget::clone()
//{
//    GameWidget *gameWidget = new GameWidget();
//    gameWidget->setBoard(board);
//    return gameWidget;
//}

//void GameWidget::setBoard(int b[4][4])
//{
//    for(int x = 0; x < 4; x++) {
//        for(int y = 0; y < 4; y++) {
//            board[x][y] = b[x][y];
//        }
//    }
//}



