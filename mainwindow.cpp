#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->restart, SIGNAL(clicked(bool)), this, SLOT(restartGame()));
    connect(ui->regret, SIGNAL(clicked(bool)), this, SLOT(regret()));
    connect(ui->PVP, SIGNAL(clicked(bool)), this, SLOT(PVPfun()));
    connect(ui->PVE,SIGNAL(clicked(bool)),this,SLOT(PVEfun()));

    GameModel game; // 默认构造
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    // 反走样
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 设置画笔颜色
    QPen tempPen(QColor(0, 160, 230));
    tempPen.setWidth(3);
    //绘制外边框
    painter.setPen(tempPen);
    const int boardOffset = 10;
    painter.drawLine(startX - boardOffset, startY - boardOffset, startX + boardOffset + boxWidth, startY - boardOffset);
    painter.drawLine(startX - boardOffset, startY + boxWidth + boardOffset, startX + boardOffset + boxWidth, startY + boardOffset + boxWidth);
    painter.drawLine(startX - boardOffset, startY - boardOffset, startX - boardOffset, startY + boardOffset + boxWidth);
    painter.drawLine(startX + boardOffset + boxWidth, startY - boardOffset, startX + boxWidth + boardOffset, startY + boardOffset + boxWidth);

    //绘制棋盘
    tempPen.setWidth(1);
    painter.setPen(tempPen);
    // 绘制直线

    //横着
    for (int i = 0; i <= boxNum; ++i)
    {
        painter.drawLine(startX, startY + i * gap, startX + boxWidth, startY + i * gap);
        painter.drawText(startX - 27, startY + i * gap + 5, QString::number(i));
    }
    //竖着
    for (int j = 0; j <= boxNum; ++j)
    {
        painter.drawLine(startX + j * gap, startY, startX + j * gap, startY + boxWidth);
        painter.drawText(startX + j * gap - 5, startY - 15, QString::number(j));
    }

    QPen whitePen(QColor(255, 255, 255));
    QPen blackPen(QColor(0, 0, 0));
    QBrush whilteBrush(QBrush(QColor(255, 255, 255), Qt::SolidPattern));
    QBrush blackBrush(QBrush(QColor(0, 0, 0), Qt::SolidPattern));
    for (int i = 0; i < boxNum + 1; ++i)
    {
        for (int j = 0; j < boxNum + 1; ++j)
        {
            if (game.chess.gomoku[i][j] == 1)
            {
                painter.setPen(whitePen);
                painter.setBrush(whilteBrush);
                painter.drawEllipse(QPoint(startX + i * gap, startY + j * gap), (int)(gap * 0.35), (int)(gap * 0.35));
            }
            else if (game.chess.gomoku[i][j] == 2)
            {
                painter.setPen(blackPen);
                painter.setBrush(blackBrush);
                painter.drawEllipse(QPoint(startX + i * gap, startY + j * gap), (int)(gap * 0.35), (int)(gap * 0.35));
            }
        }
    }
    //绘制落子点
    QPen redPen(QColor(255, 0, 0));
    painter.setPen(redPen);
    painter.setBrush(Qt::NoBrush);
    if (game.stepAlreadyMade > 0)
    {
        painter.drawRect(startX + gap * game.XStack[game.stepAlreadyMade] - (int)(gap * 0.5), startY + gap * game.YStack[game.stepAlreadyMade] - (int)(gap * 0.5),
                         gap, gap);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (game.gameOver /*|| AIIsThinking*/)
        return;
    //获得当前坐标
    int x = e->x() - (startX - gap / 2);
    int y = e->y() - (startY - gap / 2);
    if (x <= 0)
        x = 0;
    else
    {
        x /= gap;
    }
    if (y <= 0)
        y = 0;
    else
    {
        y /= gap;
    }
    if (game.nowWhite)
        QOUT << "白子 "
             << "x=" << x << "  y=" << y;
    else
        QOUT << "黑子  "
             << "x=" << x << "  y=" << y;


    // 点击后更新信息
    if (x > boxNum || y > boxNum)
        return;

    if (game.chess.gomoku[x][y] == 0)
    {
        game.chess.gomoku[x][y] = game.nowWhite ? 1 : 2;


        ++game.stepAlreadyMade;
        game.XStack[game.stepAlreadyMade] = x;
        game.YStack[game.stepAlreadyMade] = y;
    }
    else
        return;

    game.stepAll.insert({game.stepAlreadyMade, GameModel::SingleStep({x, y}, game.nowWhite)});


    // 判断此次操作是否结束游戏
    if (game.judge(x, y, game.nowWhite))
    {
        game.gameOver = true;
        if (game.nowWhite)
        {
            ui->gameStatus->setText(tr("白棋获胜！"));
        }
        else
        {
            ui->gameStatus->setText(tr("黑棋获胜！"));
        }
        //      qDebug()<<"win !!!"<<endl;
        this->repaint();
        game.AIIsThinking = false;
        return;
    }

    for (auto item : game.stepAll)
    {
        QOUT << item.first << ' ' << item.second.point.first << " " <<item.second.point.second << " " << item.second.isWhite;
    }

    // 判断结束后换手等操作
    if (game.stepAlreadyMade >= 15 * 15)
    {
        ui->gameStatus->setText(tr("平局！"));
        return;
    }
    game.nowWhite = !game.nowWhite;
    if (game.nowWhite)
        ui->who->setText(tr("请白棋落子"));
    else
        ui->who->setText(tr("请黑棋落子"));
    //  update();
    this->repaint();

    
    if (!game.isPVP)
    {
        //对手下子完毕
        ui->gameStatus->setText(tr("AI思考中..."));
        //多线程版本代码
        game.AIIsThinking = true;
        emit AIShouldMove();
    }
    return;
}

void MainWindow::PVPfun()
{
    restartGame();
    game.isPVP = true;
}

void MainWindow::PVEfun()
{
    restartGame();
    game.isPVP = false;
}

void MainWindow::restartGame()
{
    game.~GameModel();
    game = GameModel();

    if (game.nowWhite)
        ui->who->setText(tr("请白棋落子"));
    else
        ui->who->setText(tr("请黑棋落子"));
    ui->gameStatus->setText(tr("游戏进行中"));
    update();
}
void MainWindow::regret()
{
//    if (AIIsThinking)
//        return;
    if (game.stepAlreadyMade <= 0)
    {
        QMessageBox::warning(this, tr("五子棋"), tr("不能悔棋了"));
        return;
    }
    game.chess.gomoku[game.XStack[game.stepAlreadyMade]][game.YStack[game.stepAlreadyMade]] = 0;
    --game.stepAlreadyMade;
    game.chess.gomoku[game.XStack[game.stepAlreadyMade]][game.YStack[game.stepAlreadyMade]] = 0;
    --game.stepAlreadyMade;
    game.nowWhite = false;
    game.gameOver = false;
    if (game.nowWhite)
        ui->who->setText(tr("请白棋落子"));
    else
        ui->who->setText(tr("请黑棋落子"));

    ui->gameStatus->setText(tr("棋局进行中"));

    update();
}
