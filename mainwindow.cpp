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

    stepAlreadyMade = 0;
    AIIsThinking = false;
    gameOver = false;
    isPVP = true;
    nowWhite = true;
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
            if (chess.getChess(i,j) == 1)
            {
                painter.setPen(whitePen);
                painter.setBrush(whilteBrush);
                painter.drawEllipse(QPoint(startX + i * gap, startY + j * gap), (int)(gap * 0.35), (int)(gap * 0.35));
            }
            else if (chess.getChess(i,j) == 2)
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
    if (gameOver || AIIsThinking)
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


    /// 点击后更新信息
    if (x > boxNum || y > boxNum)
        return;

    if (chess.getChess(x,y) == 0)
    {
        chess.setChess(x,y,(nowWhite ? 1 : 2));

        ++stepAlreadyMade;
        XStack[stepAlreadyMade] = x;
        YStack[stepAlreadyMade] = y;
    }
    else
        return;


//    mcts.tree.setRoot(1);
//    auto root = mcts.tree.getRoot();

    /// 判断此次操作是否结束游戏
    if (game.judge(chess, x, y, nowWhite))
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
        QOUT << game.judgeAll(chess);
        this->repaint();
        AIIsThinking = false;
        return;
    }

    /// 判断结束后换手等操作
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
    this->repaint();

    
    //////////////////////////////////////////// @details AI落子
    if (!isPVP)
    {
        //对手下子完毕
        ui->gameStatus->setText(tr("AI思考中..."));
        AIIsThinking = true;

        // 执行MCTS！！！
        chess = mcts.UCTsearch(chess, chess.getLastPoint(), 1);

        /// AI赢了
        if (game.judgeAll(chess))
        {
            gameOver = true;
            ui->gameStatus->setText(tr("黑棋获胜！"));
            this->repaint();
            AIIsThinking = false;
            return;
        }

//        // 绘图
//        ++stepAlreadyMade;
//        XStack[stepAlreadyMade] =
//        YStack[stepAlreadyMade] =

        nowWhite = !nowWhite;
        if (nowWhite)
            ui->who->setText(tr("请白棋落子"));
        else
            ui->who->setText(tr("请黑棋落子"));


///////// 测试休眠2秒
//        QEventLoop eventloop;
//        QTimer::singleShot(2000, &eventloop, SLOT(quit()));
//        eventloop.exec();

        this->repaint();


        ui->gameStatus->setText(tr("棋局进行中"));
        AIIsThinking = false;
    }
        //////////////////////////////////////////// AI落子结束


    return;
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
    game.clearChess(&chess);

    isPVP = true;
    gameOver = false;           ///< 判断游戏是否结束
    nowWhite = true;           ///< 记录当前是否是白子
    AIIsThinking = false;
    stepAlreadyMade = 0;
    memset(XStack,0,sizeof XStack);
    memset(YStack,0,sizeof YStack);

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

    chess.setChess(XStack[stepAlreadyMade],YStack[stepAlreadyMade], 0);
    --stepAlreadyMade;
    chess.setChess(XStack[stepAlreadyMade],YStack[stepAlreadyMade], 0);
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
