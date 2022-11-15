#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GameModel.h"
#include "QTimer"
#include "Chess.hpp"
#include "MCTS.h"
#include "MultiwayTree.hpp"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /// @brief 前端函数
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);

private:
    Ui::MainWindow *ui;


    Chess chess;
    MCTS mcts;
    GameModel game;

    bool isPVP;              ///< 判断是不是PVP模式
    bool gameOver;           ///< 判断游戏是否结束
    bool nowWhite;           ///< 记录当前是否是白子
    int stepAlreadyMade;     ///< 已经走的步数
    bool AIIsThinking;       ///< 判断是不是AI在运行
    int XStack[250];         ///< 画图、悔棋用 从1开始算
    int YStack[250];         ///< 画图、悔棋用 从1开始算


/// @biref 信号槽用的函数
public slots:
    void restartGame();
    void regret();
    void PVPfun();
    void PVEfun();
};

#endif // MAINWINDOW_H
