#ifndef CHESS_H
#define CHESS_H

#include <utility>
#include <vector>

constexpr static int gap = 40;
constexpr static int boxNum = 14;
constexpr static int boxWidth = gap * boxNum;
constexpr static int startX = 60;
constexpr static int startY = 60;

class Chess
{
private:
    int gomoku[boxNum + 1][boxNum + 1];
    std::pair<int, int> lastPoint;

public:
    /// player = 1白棋 / 2黑棋

    void setChess(int x, int y, int player);
    int getChess(int x,int y);

    /// 返回最后一次下的棋子坐标
    std::pair<int, int> getLastPoint();
    std::vector<std::vector<int>> convertGomokuToVec();

public:
    Chess();
    Chess(int rgomoku[boxNum + 1][boxNum + 1]);



public:

    friend bool operator<(Chess x, Chess y)//用于搜索棋盘状态
    {
        for (int i = 0; i <= boxNum; i++)
        {
            for (int j = 0; j <= boxNum; j++)
            {
                if (x.gomoku[i][j] < y.gomoku[i][j])
                    return 1;
                else if (x.gomoku[i][j] > y.gomoku[i][j])
                    return 0;
            }
        }
        return 0;
    }

    friend bool operator==(Chess x, Chess y)//用于判断棋盘状态是否相等
    {
        for (int i = 0; i <= boxNum; i++)
        {
            for (int j = 0; j < boxNum; j++)
            {
                if (x.gomoku[i][j] != y.gomoku[i][j])
                    return 0;
            }
        }
        return 1;
    }
};

#endif // CHESS_H
