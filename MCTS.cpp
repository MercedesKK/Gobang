#include "MCTS.h"
#include <QDebug>
#include <cmath>
#define QOUT qDebug()



//////////////////////////////////////////////////////
////              AI Object
//////////////////////////////////////////////////////

/// player 传入的是2
Chess MCTS::UCTsearch(Chess chess, std::pair<int, int> center, int player)
{
//    if (checkFastwin_1(chess).first)
//    {
//        Chess ans = chess;
//        std::pair<int, int> u = checkFastwin_1(chess).second;
//        ans.setChess(u.first, u.second, player + 1);
//        return ans;
//    }
//    if (checkFastwin_2(chess).first)
//    {
//        Chess ans = chess;
//        std::pair<int, int> u = checkFastwin_2(chess).second;
//        ans.setChess(u.first, u.second, player + 1);
//        return ans;
//    }

    if (mp.find(chess) == mp.end())
        initChess(chess);

    fa.clear();
    mp.clear();

    int cnt = 0; // 选择次数
    while (cnt <= selectNum)
    {
        cnt++;

        std::pair<Chess, int> selectPoint = treePolicy(chess, center, 1);

        for (int j = 1; j <= 1; j++)
        {
            double s = defaultPolicy(selectPoint.first, selectPoint.second ^ 1);
            backUp(selectPoint.first, chess, s);
        }
    }

    /// test 所有节点的properity
    int num = 0;
    for (auto item : mp)
    {
        QOUT << num++ << ":" << item.second.value << " " << item.second.mockNum;
    }

    Chess ans = bestChild(chess, player);
    return ans;
}

int MCTS::isTerminal(Chess x)
{
    for (int i = 0; i <= boxNum; i++)
        for (int j = 0; j <= boxNum; j++)
            if (!x.getChess(i, j))
                return 0;

    return 1;
}

std::pair<Chess, int> MCTS::treePolicy(Chess chess, std::pair<int, int> center, int nowblack)
{
    while (!isTerminal(chess) && !GameModel::judgeAll(chess))
    {
        int x1 = std::max(0, center.first - searchRange);
        int x2 = std::min(boxNum, center.first + searchRange);
        int y1 = std::max(0, center.second - searchRange);
        int y2 = std::min(boxNum, center.second + searchRange);

        if (cntNum(chess, x1, x2, y1, y2) + mp[chess].vec.size() < (x2 - x1 + 1) * (y2 - y1 + 1))
        {
            return std::make_pair(expandNode(chess, center, nowblack), nowblack);
        }
        else
        {
            Chess y = chess;
            std::vector<Chess>::iterator it;
            if (mp[y].vec.size() == 0)break;
            double maxn = - 0x3f3f3f3f - 1;
            for (it = mp[y].vec.begin(); it != mp[y].vec.end(); it++)
            {
                if (UCB(*it, nowblack) >= maxn)
                {
                    maxn = UCB(*it, nowblack);
                    chess = *it;
                }
            }
            fa[chess] = y;
        }
        nowblack ^= 1;
    }
}

int MCTS::cntNum(Chess chess, int x1, int x2, int y1, int y2)
{
    int sum = 0;
    for (int i  = x1; i <= x2; i++)
        for (int j = y1; j <= y2; j++)
            if (chess.getChess(i, j))
                sum++;

    return sum;
}

Chess MCTS::expandNode(Chess chess, std::pair<int, int> center, int nowblack)
{
    Chess y = chess;

    int x1 = std::max(0, center.first - searchRange);
    int x2 = std::min(boxNum, center.first + searchRange);
    int y1 = std::max(0, center.second - searchRange);
    int y2 = std::min(boxNum, center.second + searchRange);

    int putCnt = 0;
    while (putCnt <= 10000)
        {
            int i = x1 + rand() % (x2 - x1 + 1);
            int j = y1 + rand() % (y2 - y1 + 1);
            int o = chess.getChess(i, j);
            y.setChess(i, j, nowblack + 1);
            if (!chess.getChess(i, j) && mp.find(y) == mp.end())
            {
                initChess(y);
                mp[chess].vec.push_back(y);
                fa[y] = chess;
                return y;
            }
            y.setChess(i, j, o);
            putCnt++;
        }

}

Chess MCTS::bestChild(Chess chess, int nowblack)
{
    Chess ans;
    std::vector<Chess>::iterator it;
    double maxn = -0x3f3f3f3f - 1; /// 比最小值还小才行
    for (it = mp[chess].vec.begin(); it != mp[chess].vec.end(); it++)
    {
        if (UCB(*it, nowblack) >= maxn)
        {
            maxn = UCB(*it, nowblack);
            ans = *it;
        }
    }
    return ans;
}

double MCTS::defaultPolicy(Chess chess, int nowblack)
{
    while (1)
    {
        if (GameModel::judgeAll(chess) || isTerminal(chess))
            break;
        std::pair<int, int> h = calCenter(chess);

        int randNum = rand() % 100;
        int i = 0, j = 0;
        if (randNum < 75)
        {
            i = std::min(std::max(0, h.first - searchRange + rand() % (searchRange * 2 + 1)), boxNum);
            j = std::min(std::max(0, h.second - searchRange + rand() % (searchRange * 2 + 1)), boxNum);
        }
        else
        {
            i = rand() % (boxNum + 1);
            j = rand() % (boxNum + 1);
        }
        if (!chess.getChess(i, j))
        {
            chess.setChess(i, j, nowblack + 1);
            nowblack ^= 1;
        }
    }

    if (GameModel::judgeAll(chess) == 1)
        return -1;
    else if (GameModel::judgeAll(chess) == 2)
        return 1;
    else
        return 0;
}

void MCTS::backUp(Chess x, Chess y, int value)
{
    mp[x].value += value;
    mp[x].mockNum++;
    while (!(x == y))
    {
        if (fa.find(x) == fa.end())
            break;
        x = fa[x];
        mp[x].value += value;
        mp[x].mockNum++;
    }
}

double MCTS::UCB(Chess chess, int player)
{
    if (mp[chess].mockNum == 0)
        return 0;
    double val = mp[chess].value, mocknum = mp[chess].mockNum;
    if (val + mocknum == 0)
        return -0x3f3f3f3f;
    if (player == 1)    // black
        return val / mocknum + sqrt(log(mocknum) / mocknum);
    else if (player == 0) // white
        return - val / mocknum + sqrt(log(mocknum) / mocknum);
}

std::pair<int, int> MCTS::calCenter(Chess chess)
{
    int cnt = 0, p1 = 0, p2 = 0;
    for (int i = 0; i <= boxNum; i++)
        for (int j = 0; j <= boxNum; j++)
        {
            if (chess.getChess(i, j))
            {
                cnt++;
                p1 += i;
                p2 += j;
            }
        }

    p1 = std::max(0, p1 / cnt);
    p2 = std::max(0, p2 / cnt);
    return std::make_pair(p1, p2);
}

void MCTS::initChess(Chess chess)
{
    Properity p;
    p.value = 0, p.mockNum = 0;
    mp[chess] = p;
}

//std::pair<int, std::pair<int, int>> MCTS::checkFastwin_1(Chess chess)
//{
//    Chess y = chess;
//    for (int i = 0; i <= boxNum; i++)
//    {
//        for (int j = 0; j <= boxNum; j++)
//        {
//            if (!chess.getChess(i, j))
//            {
//                chess.setChess(i, j, 2);
//                if (GameModel::judgeAll(chess) == 2)
//                    return std::make_pair(2, std::make_pair(i, j));
//                chess.setChess(i, j, 0);
//            }
//        }
//    }
//    for (int i = 0; i <= boxNum; i++)
//    {
//        for (int j = 0; j <= boxNum; j++)
//        {
//            if (!y.getChess(i, j))
//            {
//                y.setChess(i, j, 1);
//                if (GameModel::judgeAll(y) == 1)
//                    return std::make_pair(1, std::make_pair(i, j));
//                y.setChess(i, j, 0);
//            }
//        }
//    }
//   return std::make_pair(0, std::make_pair(0, 0));
//}

//std::pair<int, std::pair<int, int>> MCTS::checkFastwin_2(Chess chess)
//{
//    Chess y1 = chess, y2 = chess;
//        for (int i = 0; i <= boxNum; i++)
//        {
//            for (int j = 0; j <= boxNum; j++)
//            {
//                if (!chess.getChess(i, j))
//                {
//                    chess.setChess(i, j, 2);
//                    int flag = 1;
//                    for (int k1 = 0; k1 <= boxNum; k1++)
//                    {
//                        for (int k2 = 0; k2 <= boxNum; k2++)
//                        {
//                            if (!chess.getChess(k1, k2))
//                            {
//                                chess.setChess(i, j, 1);
//                                if (checkFastwin_1(chess).first != 2)
//                                {
//                                    flag = 0;
//                                    chess.setChess(i, j, 0);
//                                    break;
//                                }
//                                else
//                                    chess.setChess(i, j, 0);
//                            }
//                        }
//                        if (!flag)
//                            break;
//                    }
//                    if (flag)
//                        return std::make_pair(2, std::make_pair(i, j));
//                    chess.setChess(i, j, 0);
//                }
//            }
//        }

//        std::vector<std::pair<int, int>> vec;

//        for (int i = 0; i <= boxNum; i++)
//        {
//            for (int j = 0; j <= boxNum; j++)
//            {
//                if (!y1.getChess(i, j))
//                {
//                    y1.setChess(i, j, 1);
//                    int flag = 1;
//                    for (int k1 = 0; k1 <= boxNum; k1++)
//                    {
//                        for (int k2 = 0; k2 <= boxNum; k2++)
//                        {
//                            if (!y1.getChess(k1, k2))
//                            {
//                                y1.setChess(k1, k2, 2);
//                                if (checkFastwin_1(y1).first != 1)
//                                {
//                                    flag = 0;
//                                    y1.setChess(k1, k2, 0);
//                                    break;
//                                }
//                                else
//                                    y1.setChess(k1, k2, 0);
//                            }
//                        }
//                        if (!flag)
//                            break;
//                    }
//                    if (flag)
//                         return std::make_pair(1, std::make_pair(i, j));
//                    y1.setChess(i, j, 0);
//                }
//            }
//        }

//        std::vector<std::pair<int, int>>::iterator it;

//        std::pair<int, int> ret;
//        int minn = 1e9 + 7;

//        for (it = vec.begin(); it != vec.end(); it++)
//        {
//            std::pair<int, int> k = *it;
//            if ((k.first - calCenter(y2).first) + (k.second - calCenter(y2).second) < minn)
//            {
//                minn = (k.first - calCenter(y2).first) + (k.second - calCenter(y2).second);
//                ret = k;
//            }
//        }

//        if (vec.size())
//            return std::make_pair(1, ret);

//        return std::make_pair(0, std::make_pair(0, 0));
//}
