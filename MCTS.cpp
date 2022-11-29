#include "MCTS.h"
#include <QDebug>
#include <cmath>
#define QOUT qDebug()

std::mutex mtx;

/// player 传入的是2
Chess MCTS::UCTsearch(Chess chess, std::pair<int, int> center, int player)
{
    if (mp.find(chess) == mp.end())
        initChess(chess);

    fa.clear();                             /////////////// 注释掉

    ConcurrencyCaluate choose;
    goodNext = choose.bestChildPro(chess);
    root = chess;
    mp.clear();


                                            // if (mp.find(chess) == mp.end())
                                            // initChess(chess);
                                            // mp.clear();
                                            //    tree.remove(tree.getRoot()->_val.second);
                                            //    tree.setRoot(chess);



    chooseCnt = 0; // 选择次数
    while (chooseCnt <= 300)
    {
        chooseCnt++;
        std::pair<Chess, int> selectPoint = treePolicy(chess, center, 1);

        int val;
        defaultPolicy(selectPoint.first, selectPoint.second ^ 1, val);
        backUp(selectPoint.first, chess, val);
    }

    for (auto& it : mp)
    {
        QOUT << it.second.value;
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

                        //        if (cntNum(chess, x1, x2, y1, y2) + tree.getChildren(tree.find(chess)).size() < (x2 - x1 + 1) * (y2 - y1 + 1))
        if (cntNum(chess, x1, x2, y1, y2) + mp[chess].vec.size() < (x2 - x1 + 1) * (y2 - y1 + 1))       /////////////// 注释掉
        {
            return std::make_pair(expandNode(chess, center, nowblack), nowblack);
        }
        else
        {
            Chess y = chess;
            std::vector<Chess>::iterator it;
                        //            if (tree.getChildren(tree.find(y)).size() == 0)break;
            if (mp[y].vec.size() == 0)break;                                             /////////////// 注释掉
            double maxn = - 0x3f3f3f3f - 1;
                        //for (it = tree.getChildren(tree.find(y)).begin(); it != tree.getChildren(tree.find(y)).end(); it++)
            for (it = mp[y].vec.begin(); it != mp[y].vec.end(); it++)                   /////////////// 注释掉
            {
                if (UCB(*it, nowblack) >= maxn)
                {
                    maxn = UCB(*it, nowblack);
                    chess = *it;
                }
            }
            fa[chess] = y;                              /////////////// 注释掉
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
                if (goodNext == y) // 特殊情况
                {
                    initChess(y);
                    mp[y].value += 1000;
                    mp[chess].vec.push_back(goodNext);
                    fa[y] = chess;
                    return y;
                }

                                                //initChess(y);
                                                //tree.addChild(tree.find(chess), y);
                initChess(y);                   /////////////// 注释掉
                mp[chess].vec.push_back(y);     /////////////// 注释掉
                fa[y] = chess;                  /////////////// 注释掉
                return y;
            }
            y.setChess(i, j, o);
            putCnt++;
        }

}

Chess MCTS::bestChild(Chess chess, int nowblack)
{
    Chess ans = chess;
    std::vector<Chess>::iterator it;
    double maxn = -0x3f3f3f3f - 1; /// 比最小值还小才行
                            //for (it = tree.getChildren(tree.find(chess)).begin(); it != tree.getChildren(tree.find(chess)).end(); it++)
    for (it = mp[chess].vec.begin(); it != mp[chess].vec.end(); it++)   /////////////// 注释掉
    {
        if (UCB(*it, nowblack) >= maxn)
        {
            maxn = UCB(*it, nowblack);
            ans = *it;
        }
    }
    if (chooseCnt >= 25)
    {
        std::vector<Chess>::iterator iter = std::find(mp[root].vec.begin(), mp[root].vec.end(), goodNext);
        if (iter == mp[root].vec.end())
            ans = goodNext;
    }

    return ans;
}

Chess ConcurrencyCaluate::bestChildPro(Chess chess)
{
    MCTS::initDoubleVector(gameMapVec);
    MCTS::initDoubleVector(scoreMapVec);

    gameMapVec = chess.convertGomokuToVec();
    calculateScore();

    // 从评分中找出最大分数的位置
    int maxScore = 0;
    std::vector<std::pair<int, int>> maxPoints;

    for (int row = 0; row <= boxNum; row++)
        for (int col = 0; col <= boxNum; col++)
        {
            // 前提是这个坐标是空的
            if (gameMapVec[row][col] == 0)
            {
                if (scoreMapVec[row][col] > maxScore)          // 找最大的数和坐标
                {
                    maxPoints.clear();
                    maxScore = scoreMapVec[row][col];
                    maxPoints.push_back(std::make_pair(row, col));
                }
                else if (scoreMapVec[row][col] == maxScore)     // 如果有多个最大的数，都存起来
                    maxPoints.push_back(std::make_pair(row, col));
            }
        }

    int index = rand() % maxPoints.size();

    std::pair<int, int> pointPair = maxPoints.at(index);

    chess.setChess(pointPair.first, pointPair.second, 2); // AI棋子

    return chess;
}

void ConcurrencyCaluate::calculateScore()
{
    // 统计玩家或者电脑连成的子
    int personNum = 0; // 玩家连成子的个数
    int botNum = 0; // AI连成子的个数
    int emptyNum = 0; // 各方向空白位的个数

    // 清空评分数组
    scoreMapVec.clear();
    for (int i = 0; i <= boxNum; i++)
    {
        std::vector<int> lineScores;
        for (int j = 0; j <= boxNum; j++)
            lineScores.push_back(0);
        scoreMapVec.push_back(lineScores);
    }

    // 计分（此处是完全遍历，其实可以用bfs或者dfs加减枝降低复杂度，通过调整权重值，调整AI智能程度以及攻守风格）
    for (int row = 0; row <= boxNum; row++)
        for (int col = 0; col <= boxNum; col++)
        {
            // 空白点就算
            if (row > 0 && col > 0 &&
                    gameMapVec[row][col] == 0)
            {
                // 遍历周围八个方向
                for (int y = -1; y <= 1; y++)
                    for (int x = -1; x <= 1; x++)
                    {
                        // 重置
                        personNum = 0;
                        botNum = 0;
                        emptyNum = 0;

                        // 原坐标不算
                        if (!(y == 0 && x == 0))
                        {
                            // 每个方向延伸4个子

                            // 对玩家白子评分（正反两个方向）
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y >= 0 && row + i * y <= boxNum &&
                                        col + i * x >= 0 && col + i * x <= boxNum &&
                                        gameMapVec[row + i * y][col + i * x] == 1) // 玩家的子
                                {
                                    personNum++;
                                }
                                else if (row + i * y >= 0 && row + i * y <= boxNum &&
                                         col + i * x >= 0 && col + i * x <= boxNum &&
                                         gameMapVec[row + i * y][col + i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y >= 0 && row - i * y <= boxNum &&
                                        col - i * x >= 0 && col - i * x <= boxNum &&
                                        gameMapVec[row - i * y][col - i * x] == 1) // 玩家的子
                                {
                                    personNum++;
                                }
                                else if (row - i * y >= 0 && row - i * y <= boxNum &&
                                         col - i * x >= 0 && col - i * x <= boxNum &&
                                         gameMapVec[row - i * y][col - i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            if (personNum == 1)                      // 杀二
                                scoreMapVec[row][col] += 10;
                            else if (personNum == 2)                 // 杀三
                            {
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 30;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 40;
                            }
                            else if (personNum == 3)                 // 杀四
                            {
                                // 量变空位不一样，优先级不一样
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 60;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 110;
                            }
                            else if (personNum == 4)                 // 杀五
                                scoreMapVec[row][col] += 10100;

                            // 进行一次清空
                            emptyNum = 0;

                            // 对AI黑子评分
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y >= 0 && row + i * y <= boxNum &&
                                        col + i * x >= 0 && col + i * x <= boxNum &&
                                        gameMapVec[row + i * y][col + i * x] == 1) // 玩家的子
                                {
                                    botNum++;
                                }
                                else if (row + i * y >= 0 && row + i * y <= boxNum &&
                                         col + i * x >= 0 && col + i * x <= boxNum &&
                                         gameMapVec[row +i * y][col + i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y >= 0 && row - i * y <= boxNum &&
                                        col - i * x >= 0 && col - i * x <= boxNum &&
                                        gameMapVec[row - i * y][col - i * x] == -1) // AI的子
                                {
                                    botNum++;
                                }
                                else if (row - i * y >= 0 && row - i * y <= boxNum &&
                                         col - i * x >= 0 && col - i * x <= boxNum &&
                                         gameMapVec[row - i * y][col - i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            if (botNum == 0)                      // 普通下子
                                scoreMapVec[row][col] += 5;
                            else if (botNum == 1)                 // 活二
                                scoreMapVec[row][col] += 10;
                            else if (botNum == 2)
                            {
                                if (emptyNum == 1)                // 死三
                                    scoreMapVec[row][col] += 25;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 50;  // 活三
                            }
                            else if (botNum == 3)
                            {
                                if (emptyNum == 1)                // 死四
                                    scoreMapVec[row][col] += 55;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 100; // 活四
                            }
                            else if (botNum >= 4)
                                scoreMapVec[row][col] += 10000;   // 活五

                        }
                    }

            }
        }
}

void MCTS::defaultPolicy(Chess chess, int nowblack, int& value)
{
    while (1)
    {
        if (GameModel::judgeAll(chess) || isTerminal(chess))
            break;
        std::pair<int, int> h = calCenter(chess);

        //if (nowblack)
        //{
        //    ConcurrencyCaluate cal;
        //    chess = cal.bestChildPro(chess);
        //    value = 1;
        //    return;
        //    nowblack ^= 1;
        //}

        int randNum = rand() % 100;
        int i = 0, j = 0;
        if (randNum < 50)
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
        value = -1;
    else if (GameModel::judgeAll(chess) == 2)
        value = 1;
    else
        value = 0;
}

void MCTS::backUp(Chess x, Chess y, int value)
{
    mp[x].value += value;
    mp[x].mockNum++;
    while (!(x == y))
    {
        if (fa.find(x) == fa.end()) /////////////// 注释掉
            break;                  /////////////// 注释掉
        x = fa[x];                  /////////////// 注释掉

                                    //        if (x == tree.getRoot()->_val.second)
                                    //            break;
                                    //       x = tree.find(x)->_parent->_val.second;

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

void MCTS::initDoubleVector(std::vector<std::vector<int>>& rhs)
{
    rhs.clear();
    for (int i = 0; i <= boxNum; i++)
    {
        std::vector<int> lineBoard;
        for (int j = 0; j <= boxNum; j++)
            lineBoard.push_back(0);
        rhs.push_back(lineBoard);
    }
}