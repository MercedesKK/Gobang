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
#define QOUT qDebug()

constexpr static int gap = 40;
constexpr static int boxNum = 14;
constexpr static int boxWidth = gap * boxNum;
constexpr static int startX = 60;
constexpr static int startY = 60;

class Chess
{
public:
    int gomoku[boxNum + 1][boxNum + 1];

public:
    Chess();
};

class GameModel
{
    friend class MainWindow;
private:
    Chess chess;

    bool isPVP;              ///< 判断是不是PVP模式
    bool gameOver;           ///< 判断游戏是否结束
    bool nowWhite;           ///< 记录当前是否是白子
    int stepAlreadyMade;     ///< 已经走的步数
    bool AIIsThinking;       ///< 判断是不是AI在运行
    // 画图、悔棋用 从1开始算
    int XStack[250];
    int YStack[250];

    /// @brief 储存该棋局面已经下棋的信息
    struct SingleStep
    {
        std::pair<int, int> point;  ///< 这一步是什么坐标
        bool isWhite;               ///< 这一步是什么棋

        SingleStep(std::pair<int,int> rhsPoint, bool rhsIsWhite): point(rhsPoint),isWhite(rhsIsWhite) {}
    };
    // 第一个模板参数代表第几步，第二个模板参数见上
    std::map<int, SingleStep> stepAll;


public:
    GameModel();

    bool judge(int x, int y, bool nowWhite); // 判断（x,y）处nowwhile是否赢
    bool judgeGameOver();  // 结束返回true
};



#endif // GAMEMODEL_H
