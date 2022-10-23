#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

enum patternID
{
    LongLink = 0,
    Active4,
    Sleep4,
    Active3,
    Sleep3,
    Active2,
    Sleep2,
    Dead4,
    Dead3,
    Dead2
};

struct step
{
    int x;
    int y;
    bool nowWhite;
    int score;
    step(int xx = 0, int yy = 0, bool nw = true)
    {
        x = xx;
        y = yy;
        nowWhite = nw;
    }
};

struct pattern
{
    std::vector<QString> whitePattern;
    std::vector<QString> blackPattern;
    int id;
    int whiteScore;
    int blackScore;
    void clear()
    {
        whitePattern.clear();
        blackPattern.clear();
    }
};

namespace Ui
{
    class MainWindow;
}

class MainWindow;
class AI : public QObject
{
    Q_OBJECT
public:
    AI();
    AI(MainWindow *p) : parentWindow(p) {}
    int count;

private:
    MainWindow *parentWindow;

public slots:
    void getAINextStep();

public:
    int getChessScore(int chess[15][15], int leftEdge = 0, int rightEdge = 14, int upEdge = 0, int downEdge = 14);
    int getLineScore(QString, int whitePatternNum[10], int blackPatternNum[15]);
    std::vector<step> getPossibleSteps(int chess[15][15], bool white, int leftEdge = 0, int rightEdge = 14, int upEdge = 0, int downEdge = 14);
    int alpha_beta(int chess[15][15], int depth, int alpha, int beta);
    bool calculateScore(std::vector<step> &possibleSteps, int chess[15][15]);
    void deleteUselessStep(std::vector<step> &possibleSteps, int chess[15][15]);
    bool reserveKillStep(std::vector<step> &possibleSteps, int chess[15][15]);
signals:
    void stepReady();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class AI;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
public slots:
    void handleAIStep();
signals:
    void AIShouldMove();

private:
    Ui::MainWindow *ui;
    QThread AIThread;

    bool isPVP;
    bool gameOver;
    int searchDepth; //搜索深度,应当是偶数
    int killStepSearchDepth;
    int stepAlreadyMade; //已经走的步数
    const int gap = 40;
    constexpr static int boxNum = 14;
    const int boxWidth = gap * boxNum;
    const int startX = 60;
    const int startY = 60;
    bool nowWhite; //记录当前是否是白子 白色=1 黑色=2
    int gomoku[boxNum + 1][boxNum + 1];
    int gomokuForPaint[boxNum + 1][boxNum + 1];
    bool judge(int x, int y, bool nowWhite);
    bool judge(int chess[15][15], int x, int y, bool nowWhite);
    int steps; //用于设定搜索范围的步数
    int AINextX;
    int AINextY;
    bool AIIsThinking;
    std::vector<pattern> patterns;      //储存算分的模式
    std::vector<pattern> lightPatterns; //排序走法用的模式
    int zobristHash;

    std::unordered_map<int, int> scoreCache;
    bool debug;

    int basicScore[15][15] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
        {0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0},
        {0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0},
        {0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0},
        {0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0},
        {0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0},
        {0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0},
        {0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0},
        {0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0},
        {0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0},
        {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    int zobristWhite[15][15];
    int zobristBlack[15][15];
    int XStack[250];    //从1开始算
    int YStack[250];    //从1开始算
    void initPattern(); //初始化算分模式
public slots:
    void restartGame();
    void regret();
    void PVPfun();
    void PVEfun();
};

#endif // MAINWINDOW_H
