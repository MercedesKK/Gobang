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
#include "Chess.h"
#include "MultiwayTree.hpp"

#define QOUT qDebug()

class GameModel
{
    friend class MainWindow;
public:
    GameModel() = default;

    void clearChess(Chess* chess);                       /// @attention 必须传入地址，不然清除不干净
    static bool judge(Chess chess,int x, int y, bool nowWhite); // 判断（x,y）处nowwhile是否赢
    static int judgeAll(Chess chess);                          // 1代表白棋赢，2代表黑棋赢，0代表没结束
};


#endif // GAMEMODEL_H
