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

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /// @brief 前端画图等函数
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);

private:
    Ui::MainWindow *ui;

    GameModel game;

/// @biref 信号槽用的函数
public slots:
    void restartGame();
    void regret();
    void PVPfun();
    void PVEfun();

signals:
    void AIShouldMove();
};

#endif // MAINWINDOW_H
