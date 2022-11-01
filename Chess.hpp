#ifndef CHESS_H
#define CHESS_H

constexpr static int gap = 40;
constexpr static int boxNum = 14;
constexpr static int boxWidth = gap * boxNum;
constexpr static int startX = 60;
constexpr static int startY = 60;

class Chess
{
private:
    int gomoku[boxNum + 1][boxNum + 1];

public:
    /// player = 1白棋 / 2黑棋
    void setChess(int x, int y, int player)
    {
        gomoku[x][y] = player;
    }

    int getChess(int x,int y)
    {
        return gomoku[x][y];
    }

public:
    Chess()
    {
        for (int i = 0; i < boxNum + 1; ++i)
            for (int j = 0; j < boxNum + 1; ++j)
                gomoku[i][j] = 0;
    }

};


#endif // CHESS_H
