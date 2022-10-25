#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GameModel.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class AI;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);

public slots:
    void handleAIStep();
signals:
    void AIShouldMove();

private:
    Ui::MainWindow *ui;

    bool isPVP;
    bool gameOver;
    bool nowWhite; //记录当前是否是白子
    bool AIIsThinking;

    int gomoku[boxNum + 1][boxNum + 1]; //白色=1 黑色=2

    bool judge(int x, int y, bool nowWhite);
    bool judge(int chess[15][15], int x, int y, bool nowWhite);

    int stepAlreadyMade; //已经走的步数
    int XStack[250];    //从1开始算
    int YStack[250];    //从1开始算

public slots:
    void restartGame();
    void regret();
    void PVPfun();
    void PVEfun();
};

#endif // MAINWINDOW_H
