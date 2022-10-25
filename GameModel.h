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

#endif // GAMEMODEL_H
