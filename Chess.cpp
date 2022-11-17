#include "Chess.h"

void Chess::setChess(int x, int y, int player)
{
    gomoku[x][y] = player;
    lastPoint = std::make_pair(x, y);
}

int Chess::getChess(int x,int y)
{
    return gomoku[x][y];
}


/// 返回最后一次下的棋子坐标
std::pair<int, int> Chess::getLastPoint()
{
    return lastPoint;
}

Chess::Chess()
{
    lastPoint = std::make_pair(0,0);

    for (int i = 0; i < boxNum + 1; ++i)
        for (int j = 0; j < boxNum + 1; ++j)
            gomoku[i][j] = 0;
}
