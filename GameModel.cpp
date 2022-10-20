#include <utility>
#include <stdlib.h>
#include <time.h>
#include "GameModel.h"

GameModel::GameModel()
{

}

void GameModel::startGame(GameType type)
{
    gameType = type;
    // ��ʼ����
    gameMapVec.clear();
    for (int i = 0; i < kBoardSizeNum; i++)
    {
        std::vector<int> lineBoard;
        for (int j = 0; j < kBoardSizeNum; j++)
            lineBoard.push_back(0);
        gameMapVec.push_back(lineBoard);
    }

    // �����AIģʽ����Ҫ��ʼ����������
    /// ***************************** AI ****************************************************************************************
    if (gameType == BOT)
    {
        scoreMapVec.clear();
        for (int i = 0; i < kBoardSizeNum; i++)
        {
            std::vector<int> lineScores;
            for (int j = 0; j < kBoardSizeNum; j++)
                lineScores.push_back(0);
            scoreMapVec.push_back(lineScores);
        }
    }

    // ������Ϊtrue,�Է���λfalse
    playerFlag = true;


}

void GameModel::updateGameMap(int row, int col)
{
    if (playerFlag)
        gameMapVec[row][col] = 1;
    else
        gameMapVec[row][col] = -1;

    // ����
    playerFlag = !playerFlag;
}

void GameModel::actionByPerson(int row, int col)
{
    updateGameMap(row, col);
}


/// ***************************** AI ****************************************************************************************

void GameModel::actionByAI(int& clickRow, int& clickCol)
{
    // ��������
    calculateScore();

    // ���������ҳ���������λ��
    int maxScore = 0;
    std::vector<std::pair<int, int>> maxPoints;

    for (int row = 1; row < kBoardSizeNum; row++)
        for (int col = 1; col < kBoardSizeNum; col++)
        {
            // ǰ������������ǿյ�
            if (gameMapVec[row][col] == 0)
            {
                if (scoreMapVec[row][col] > maxScore)          // ��������������
                {
                    maxPoints.clear();
                    maxScore = scoreMapVec[row][col];
                    maxPoints.push_back(std::make_pair(row, col));
                }
                else if (scoreMapVec[row][col] == maxScore)     // ����ж������������������
                    maxPoints.push_back(std::make_pair(row, col));
            }
        }

    // ������ӣ�����ж����Ļ�
    srand((unsigned)time(0));
    int index = rand() % maxPoints.size();

    std::pair<int, int> pointPair = maxPoints.at(index);
    clickRow = pointPair.first; // ��¼���ӵ�
    clickCol = pointPair.second;
    updateGameMap(clickRow, clickCol);
}

// ��ؼ��ļ������ֺ���
void GameModel::calculateScore()
{
    // ͳ����һ��ߵ������ɵ���
    int personNum = 0; // ��������ӵĸ���
    int botNum = 0; // AI�����ӵĸ���
    int emptyNum = 0; // ������հ�λ�ĸ���

    // �����������
    scoreMapVec.clear();
    for (int i = 0; i < kBoardSizeNum; i++)
    {
        std::vector<int> lineScores;
        for (int j = 0; j < kBoardSizeNum; j++)
            lineScores.push_back(0);
        scoreMapVec.push_back(lineScores);
    }

    // �Ʒ֣��˴�����ȫ��������ʵ������bfs����dfs�Ӽ�֦���͸��Ӷȣ�ͨ������Ȩ��ֵ������AI���̶ܳ��Լ����ط��
    for (int row = 0; row < kBoardSizeNum; row++)
        for (int col = 0; col < kBoardSizeNum; col++)
        {
            // �հ׵����
            if (row > 0 && col > 0 &&
                gameMapVec[row][col] == 0)
            {
                // ������Χ�˸�����
                for (int y = -1; y <= 1; y++)
                    for (int x = -1; x <= 1; x++)
                    {
                        // ����
                        personNum = 0;
                        botNum = 0;
                        emptyNum = 0;

                        // ԭ���겻��
                        if (!(y == 0 && x == 0))
                        {
                            // ÿ����������4����

                            // ����Ұ������֣�������������
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                    col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                    gameMapVec[row + i * y][col + i * x] == 1) // ��ҵ���
                                {
                                    personNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                    col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                    gameMapVec[row + i * y][col + i * x] == 0) // �հ�λ
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // ���߽�
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                    col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                    gameMapVec[row - i * y][col - i * x] == 1) // ��ҵ���
                                {
                                    personNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                    col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                    gameMapVec[row - i * y][col - i * x] == 0) // �հ�λ
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // ���߽�
                                    break;
                            }

                            if (personNum == 1)                      // ɱ��
                                scoreMapVec[row][col] += 10;
                            else if (personNum == 2)                 // ɱ��
                            {
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 30;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 40;
                            }
                            else if (personNum == 3)                 // ɱ��
                            {
                                // �����λ��һ�������ȼ���һ��
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 60;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 110;
                            }
                            else if (personNum == 4)                 // ɱ��
                                scoreMapVec[row][col] += 10100;

                            // ����һ�����
                            emptyNum = 0;

                            // ��AI��������
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                    col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                    gameMapVec[row + i * y][col + i * x] == 1) // ��ҵ���
                                {
                                    botNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                    col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                    gameMapVec[row + i * y][col + i * x] == 0) // �հ�λ
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // ���߽�
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                    col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                    gameMapVec[row - i * y][col - i * x] == -1) // AI����
                                {
                                    botNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                    col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                    gameMapVec[row - i * y][col - i * x] == 0) // �հ�λ
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // ���߽�
                                    break;
                            }

                            if (botNum == 0)                      // ��ͨ����
                                scoreMapVec[row][col] += 5;
                            else if (botNum == 1)                 // ���
                                scoreMapVec[row][col] += 10;
                            else if (botNum == 2)
                            {
                                if (emptyNum == 1)                // ����
                                    scoreMapVec[row][col] += 25;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 50;  // ����
                            }
                            else if (botNum == 3)
                            {
                                if (emptyNum == 1)                // ����
                                    scoreMapVec[row][col] += 55;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 100; // ����
                            }
                            else if (botNum >= 4)
                                scoreMapVec[row][col] += 10000;   // ����

                        }
                    }

            }
        }
}

bool GameModel::isWin(int row, int col)
{
    // ����б���ִ������ÿ����������ݵ�ǰ�����������5�����ӣ���һ�ַ��Ͼ���Ӯ
    // ˮƽ����
    for (int i = 0; i < 5; i++)
    {
        // ����5��������ƥ��4���ӣ�20�����
        if (col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 1] &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 2] &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 3] &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 4])
            return true;
    }

    // ��ֱ����(��������4��)
    for (int i = 0; i < 5; i++)
    {
        if (row - i > 0 &&
            row - i + 4 < kBoardSizeNum &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 1][col] &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 2][col] &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 3][col] &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 4][col])
            return true;
    }

    // ��б����
    for (int i = 0; i < 5; i++)
    {
        if (row + i < kBoardSizeNum &&
            row + i - 4 > 0 &&
            col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 1][col - i + 1] &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 2][col - i + 2] &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 3][col - i + 3] &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 4][col - i + 4])
            return true;
    }

    // ��б����
    for (int i = 0; i < 5; i++)
    {
        if (row - i > 0 &&
            row - i + 4 < kBoardSizeNum &&
            col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 1][col - i + 1] &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 2][col - i + 2] &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 3][col - i + 3] &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 4][col - i + 4])
            return true;
    }

    return false;
}

bool GameModel::isDeadGame()
{
    // ���пո�ȫ������
    for (int i = 1; i < kBoardSizeNum; i++)
        for (int j = 1; j < kBoardSizeNum; j++)
        {
            if (!(gameMapVec[i][j] == 1 || gameMapVec[i][j] == -1))
                return false;
        }
    return true;
}

