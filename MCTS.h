#ifndef AI_H
#define AI_H

///@ attention  ！！！        Xstack和Ystack在返回新节点时还没传入，方框没跟上

#include "Chess.hpp"
#include <ctime>
#include <QTime>
#include <memory>

///// 以下test
class MCTS
{
public:
    MCTS() = default;

    std::shared_ptr<Chess> MCTStest(std::shared_ptr<Chess> chess)
    {
        while (1)
        {
            int x = rand() % boxNum;
            int y = rand() % boxNum;
            if (!chess->getChess(x,y))
            {
                chess->setChess(x, y, 2);
                break;
            }
        }
        return chess;
    }
};

#endif // AI_H
