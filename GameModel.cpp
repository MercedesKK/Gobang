#include "GameModel.h"

bool GameModel::judge(std::shared_ptr<Chess> chess,int x, int y, bool nowWhite)
{
    int count = 0;
    int flag = nowWhite ? 1 : 2;
    //横向
    for (int i = ((x - 4) < 0 ? 0 : x - 4); i <= ((x + 4) > boxNum ? boxNum : x + 4); ++i)
    {
        if (chess->getChess(i, y) == flag)
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
        if (chess->getChess(x, j) == flag)
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

        if (chess->getChess(i, yy) == flag)
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

        if (chess->getChess(i, yy) == flag)
            count++;
        else
            count = 0;
        if (count == 5)
            return true;
    }

    return false;
}

int GameModel::judgeAll(std::shared_ptr<Chess> chess)
{
    for (int i = 0; i <= boxNum; i++)
        for (int j = 0; j <= boxNum; j++)
        {
            if (judge(chess, i, j, true))
                return 1;
            else if (judge(chess, i, j, false))
                return 2;
        }

    return 0;
}

void GameModel::clearChess(std::shared_ptr<Chess> chess)
{
    for (int i = 0; i <= boxNum; i++)
        for (int j = 0; j <= boxNum; j++)
        {
            chess->setChess(i, j, 0);
        }
}
