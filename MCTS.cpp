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
    Chess y = chess;
    Chess ans = y;

    tree.remove(chess);
    /// @attention 必须先清除一次tree，因为只能将当前状态作为根节点
    tree.setRoot(chess);
    chess.properity.value = 0;
    chess.properity.mockNum = 0;
    QOUT << chess.properity.mockNum;

    int cnt = 0; // 选择次数
    while (cnt <= 35)
    {
        cnt++;

        std::pair<Chess, int> selectPoint = treePolicy(chess, center, 1);

//        for (int j = 1; j <= simulationNum; j++)
//        {
//            double s = defaultPolicy(selectPoint.first, selectPoint.second ^ 1);
//            backUp(selectPoint.first, y, s);
//        }
    }

    std::vector<Chess>::iterator it;
    double maxn = UCB(*(tree.getChildren(tree.find(y)).begin()), player);

    QOUT << tree.getChildren(tree.find(y)).size();

    for (it = tree.getChildren(tree.find(y)).begin(); it != tree.getChildren(tree.find(y)).end(); it++)
    {
        if (UCB(*it, player) >= maxn)
        {
            maxn = UCB(*it, player);
            ans = *it;
        }

        QOUT << "UCB:" << UCB(*it, player);
    }
    //QOUT << tree.getRoot()->_val.second.properity.mockNum;
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

        if (cntNum(chess, x1, x2, y1, y2) + tree.getChildren(tree.find(chess)).size() < (x2 - x1 + 1) * (y2 - y1 + 1))
        {

            return std::make_pair(expandNode(chess, center, nowblack), nowblack);
        }
        else // 选择叶子节点
        {
            Chess y = chess;
            std::vector<Chess>::iterator it;
            if (tree.getChildren(tree.find(y)).size() == 0)
                break;
            double maxn = UCB(*((tree.getChildren(tree.find(y))).begin()), nowblack);
            for (it = tree.getChildren(tree.find(y)).begin(); it != tree.getChildren(tree.find(y)).end(); it++)
            {
                if (UCB(*it, nowblack) >= maxn)
                {
                    maxn = UCB(*it, nowblack);
                    chess = *it;
                }
            }
        }
        nowblack ^= 1;
    }
    QOUT << "test";
    return std::make_pair(chess, nowblack);
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
    while (putCnt <= 100000)
    {
        int i = x1 + rand() % (x2 - x1 + 1);
        int j = y1 + rand() % (y2 - y1 + 1);

        int temp = chess.getChess(i, j);
        y.setChess(i, j, nowblack + 1);

        if (!chess.getChess(i, j) && tree.find(y) == nullptr)
        {
            y.properity.value = 0, y.properity.mockNum = 0;
            tree.addChild(tree.find(chess), y);
            // QOUT << tree.getChildren(tree.find(chess)).size();
            return y;
        }
        y.setChess(i, j, temp); // 恢复
        putCnt++;
    }
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
    Chess tempx = x;
    tempx.properity.value += value;
    tempx.properity.mockNum++;
    tree.resetNodeVal(tree.find(x), tempx);
    while (!(x == y))
    {
        x = tree.find(x)->_parent->_val.second;
        Chess tempxx = x;
        tempxx.properity.value += value;
        tempxx.properity.mockNum++;
        tree.resetNodeVal(tree.find(x), tempxx);
    }
}

double MCTS::UCB(Chess chess, int player)
{
    double val = chess.properity.value;
    double mocknum = chess.properity.mockNum;

    if (mocknum == 0)
        return 0;
    if (val + mocknum == 0)
        return -0x3f3f3f3f;
    if (player == 1)    // black
        return val / mocknum + sqrt(log(val + mocknum) / mocknum);
    else if (player == 0) // white
        return - val / mocknum + sqrt(log(val + mocknum) / mocknum);
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

