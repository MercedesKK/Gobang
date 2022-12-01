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

Chess::Chess(int rgomuku[boxNum + 1][boxNum + 1])
{
    lastPoint = std::make_pair(0,0);

    for (int i = 0; i < boxNum + 1; ++i)
        for (int j = 0; j < boxNum + 1; ++j)
            gomoku[i][j] = rgomuku[i][j];
}

std::vector<std::vector<int>> Chess::convertGomokuToVec()
{
    std::vector<std::vector<int>> temp;

    temp.clear();
    for (int i = 0; i <= boxNum; i++)
    {
        std::vector<int> lineBoard;
        for (int j = 0; j <= boxNum; j++)
            lineBoard.push_back(0);
        temp.push_back(lineBoard);
    }


    for (int i = 0; i <= boxNum; i++)
        for (int j = 0; j <= boxNum; j++)
        {
            if (gomoku[i][j] == 1)
                temp[i][j] = 1;
            else if (gomoku[i][j] == 2)
                temp[i][j] = -1;
            else
                temp[i][j] = 0;
        }
    return temp;
}
