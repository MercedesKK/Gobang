#ifndef AI_H
#define AI_H

///@ attention  ！！！        Xstack和Ystack在返回新节点时还没传入，方框没跟上

#include "Chess.hpp"
#include <ctime>
#include <QTime>
#include <memory>
#include <vector>
#include "MultiwayTree.hpp"
#include "GameModel.h"

// nowWhite 用于UI判断      1为白色 0为黑色
// nowblack 用于MCTS内部判断 1为黑色 0为白色
// player   用于判断用户     0为空位 1为白色 2为黑色

class MCTS
{
public:
    using Tree = MultiwayTree<_Multiway_tree_traits<Chess>>;
    Tree tree;

    static const int searchRange = 2;       ///< 搜索范围
    static const int selectNum = 300;       ///< 选择次数
    static const int simulationNum = 15;    ///< 每个状态模拟次数


public:
    MCTS() = default;


    Chess UCTsearch(Chess chess, std::pair<int, int> center, int player);                    /// 搜索函数传入的是原来那个chess

    /// @brief 选择
    std::pair<Chess, int> treePolicy(Chess chess, std::pair<int, int> center, int nowblack);

    /// @brief 扩展
    /// @attention 目前只有5*5的范围内搜索，搜索100000次，如果运气非常非常非常非常不好就expand不到那1/25的点~
    Chess expandNode(Chess chess, std::pair<int, int> center, int nowblack);

    /// @brief 模拟    游戏至结束，黑色赢返回1，白色返回-1，和棋返回0
    double defaultPolicy(Chess chess, int nowblack);

    /// @brief 回退
    void backUp(Chess x, Chess y, int value);

    /// @brief UCB值
    double UCB(Chess chess, int player);


    std::pair<int, int> calCenter(Chess chess); // 返回当前局面平均坐标
    int isTerminal(Chess x); // 返回1时代表棋盘满了，是终端节点
    int cntNum(Chess chess, int x1, int x2, int y1, int y2);
};

#endif // AI_H
