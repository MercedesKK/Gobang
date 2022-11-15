#ifndef CHESS_H
#define CHESS_H

#include <utility>

constexpr static int gap = 40;
constexpr static int boxNum = 14;
constexpr static int boxWidth = gap * boxNum;
constexpr static int startX = 60;
constexpr static int startY = 60;

class Properity
{
public:
    double mockNum;
    double value;

    Properity(): mockNum(0), value(0) {}
};

class Chess
{
private:
    int gomoku[boxNum + 1][boxNum + 1];
    std::pair<int, int> lastPoint;

public:
    Properity properity;

public:
    /// player = 1白棋 / 2黑棋

    void setChess(int x, int y, int player)
    {
        gomoku[x][y] = player;
        lastPoint = std::make_pair(x, y);
    }

    int getChess(int x,int y)
    {
        return gomoku[x][y];
    }


    /// 返回最后一次下的棋子坐标
    std::pair<int, int> getLastPoint()
    {
        return lastPoint;
    }

    bool operator==(Chess& rhs)
    {
        for (int i = 0; i <= boxNum; i++)
        {
            for (int j = 0; j < boxNum; j++)
            {
                if (this->getChess(i, j) != rhs.getChess(i, j))return 0;
            }
        }
        return 1;
    }

public:
    Chess()
    {
        lastPoint = std::make_pair(0,0);     

        for (int i = 0; i < boxNum + 1; ++i)
            for (int j = 0; j < boxNum + 1; ++j)
                gomoku[i][j] = 0;
    }

};

#endif // CHESS_H
