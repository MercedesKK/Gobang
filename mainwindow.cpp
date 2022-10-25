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

    for (int i = 0; i < boxNum + 1; ++i)
        for (int j = 0; j < boxNum + 1; ++j)
            gomoku[i][j] = 0;

    isPVP = true;
    nowWhite = false; // true;
    gameOver = false;
    AIIsThinking = false;
    stepAlreadyMade = 0;
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
            if (gomoku[i][j] == 1)
            {
                painter.setPen(whitePen);
                painter.setBrush(whilteBrush);
                painter.drawEllipse(QPoint(startX + i * gap, startY + j * gap), (int)(gap * 0.35), (int)(gap * 0.35));
            }
            else if (gomoku[i][j] == 2)
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
    if (stepAlreadyMade > 0)
    {
        painter.drawRect(startX + gap * XStack[stepAlreadyMade] - (int)(gap * 0.5), startY + gap * YStack[stepAlreadyMade] - (int)(gap * 0.5),
                         gap, gap);
    }
}
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (gameOver /*|| AIIsThinking*/)
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
    if (nowWhite)
        QOUT << "白子 "
             << "x=" << x << "  y=" << y;
    else
        QOUT << "黑子  "
             << "x=" << x << "  y=" << y;
    if (x > boxNum || y > boxNum)
        return;

    if (gomoku[x][y] == 0)
    {
        gomoku[x][y] = nowWhite ? 1 : 2;


        ++stepAlreadyMade;
        XStack[stepAlreadyMade] = x;
        YStack[stepAlreadyMade] = y;
    }
    else
        return;

    if (judge(x, y, nowWhite))
    {
        gameOver = true;
        if (nowWhite)
        {
            ui->gameStatus->setText(tr("白棋获胜！"));
        }
        else
        {
            ui->gameStatus->setText(tr("黑棋获胜！"));
        }
        //      qDebug()<<"win !!!"<<endl;
        this->repaint();
        AIIsThinking = false;
        return;
    }
    if (stepAlreadyMade >= 15 * 15)
    {
        ui->gameStatus->setText(tr("平局！"));
        return;
    }
    nowWhite = !nowWhite;
    if (nowWhite)
        ui->who->setText(tr("请白棋落子"));
    else
        ui->who->setText(tr("请黑棋落子"));
    //  update();
    this->repaint();

    
    if (!isPVP)
    {
        //对手下子完毕
        ui->gameStatus->setText(tr("AI思考中..."));
        //多线程版本代码
        AIIsThinking = true;
        emit AIShouldMove();
    }
    return;
}

bool MainWindow::judge(int x, int y, bool nowWhite)
{
    int count = 0;
    int flag = nowWhite ? 1 : 2;
    //横向
    for (int i = ((x - 4) < 0 ? 0 : x - 4); i <= ((x + 4) > boxNum ? boxNum : x + 4); ++i)
    {
        if (gomoku[i][y] == flag)
            count++;
        else
            count = 0;
        if (count == 5)
            return true;
    }
    //纵向
    count = 0;
    for (int j = ((y - 4) < 0 ? 0 : y - 4); j <= ((y + 4) > boxNum ? boxNum : y + 4); ++j)
    {
        if (gomoku[x][j] == flag)
            count++;
        else
            count = 0;
        if (count == 5)
            return true;
    }
    //左下到右上
    count = 0;
    for (int i = ((x - 4) < 0 ? 0 : x - 4); i <= ((x + 4) > boxNum ? boxNum : x + 4); ++i)
    {
        int yy = y + i - x;

        if (yy < 0)
            continue;
        else if (yy > boxNum)
            break;

        if (gomoku[i][yy] == flag)
            count++;
        else
            count = 0;
        if (count == 5)
            return true;
    }
    //左上到右下
    count = 0;
    for (int i = ((x - 4) < 0 ? 0 : x - 4); i <= ((x + 4) > boxNum ? boxNum : x + 4); ++i)
    {
        int yy = y + x - i;

        if (yy > boxNum)
            continue;
        else if (yy < 0)
            break;

        if (gomoku[i][yy] == flag)
            count++;
        else
            count = 0;
        if (count == 5)
            return true;
    }

    return false;
}

bool MainWindow::judge(int chess[15][15], int x, int y, bool nowWhite)
{
    int count = 0;
    int flag = nowWhite ? 1 : 2;
    //横向
    for (int i = ((x - 4) < 0 ? 0 : x - 4); i <= ((x + 4) > boxNum ? boxNum : x + 4); ++i)
    {
        if (chess[i][y] == flag)
            count++;
        else
            count = 0;
        if (count == 5)
            return true;
    }
    //纵向
    count = 0;
    for (int j = ((y - 4) < 0 ? 0 : y - 4); j <= ((y + 4) > boxNum ? boxNum : y + 4); ++j)
    {
        if (chess[x][j] == flag)
            count++;
        else
            count = 0;
        if (count == 5)
            return true;
    }
    //左下到右上
    count = 0;
    for (int i = ((x - 4) < 0 ? 0 : x - 4); i <= ((x + 4) > boxNum ? boxNum : x + 4); ++i)
    {
        int yy = y + i - x;

        if (yy < 0)
            continue;
        else if (yy > boxNum)
            break;

        if (chess[i][yy] == flag)
            count++;
        else
            count = 0;
        if (count == 5)
            return true;
    }
    //左上到右下
    count = 0;
    for (int i = ((x - 4) < 0 ? 0 : x - 4); i <= ((x + 4) > boxNum ? boxNum : x + 4); ++i)
    {
        int yy = y + x - i;

        if (yy > boxNum)
            continue;
        else if (yy < 0)
            break;

        if (chess[i][yy] == flag)
            count++;
        else
            count = 0;
        if (count == 5)
            return true;
    }

    return false;
}

void MainWindow::PVPfun()
{
    restartGame();
    isPVP = true;
}

void MainWindow::PVEfun()
{
    restartGame();
    isPVP = false;
}

void MainWindow::restartGame()
{
//    if (AIIsThinking)
//    {
//        //结束ai进程
//        return;
//        AIThread.terminate();
//        AIThread.wait();
//    }


    for (int i = 0; i < boxNum + 1; ++i)
    {
        for (int j = 0; j < boxNum + 1; ++j)
        {
            gomoku[i][j] = 0;
        }
    }
    nowWhite = false;
    gameOver = false;

    stepAlreadyMade = 0;

    if (nowWhite)
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
    if (stepAlreadyMade <= 0)
    {
        QMessageBox::warning(this, tr("五子棋"), tr("不能悔棋了"));
        return;
    }
    gomoku[XStack[stepAlreadyMade]][YStack[stepAlreadyMade]] = 0;


    --stepAlreadyMade;
    gomoku[XStack[stepAlreadyMade]][YStack[stepAlreadyMade]] = 0;


    --stepAlreadyMade;
    nowWhite = false;
    gameOver = false;
    if (nowWhite)
        ui->who->setText(tr("请白棋落子"));
    else
        ui->who->setText(tr("请黑棋落子"));

    ui->gameStatus->setText(tr("棋局进行中"));

    update();
}
