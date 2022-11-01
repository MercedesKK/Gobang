#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QMainWindow>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QMouseEvent>
#include <QString>
#include <vector>
#include <QMessageBox>
#include <random>
#include <ctime>
#include <map>
#include <cmath>
#include <algorithm>
#include <QThread>
#include <QTime>
#include <unordered_map>
#include <memory>
#include "Chess.hpp"
#define QOUT qDebug()

class GameModel
{
    friend class MainWindow;
public:
    GameModel() = default;

    void clearChess(std::shared_ptr<Chess> chess);
    bool judge(std::shared_ptr<Chess> chess,int x, int y, bool nowWhite); // 判断（x,y）处nowwhile是否赢
    int judgeAll(std::shared_ptr<Chess> chess);  // 1代表白棋赢，2代表黑棋赢，0代表没结束
};


#endif // GAMEMODEL_H
