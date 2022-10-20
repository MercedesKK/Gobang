#ifndef GAMEMODEL_H
#define GAMEMODEL_H

// ---- ��������Ϸģ���� ---- //
#include <vector>

// ��Ϸ���ͣ�˫�˻���AI��Ŀǰ�̶���AI�º��ӣ�
enum GameType
{
    PERSON,
    BOT
};

// ��Ϸ״̬
enum GameStatus
{
    PLAYING,
    WIN,
    DEAD
};

// ���̳ߴ�
const int kBoardSizeNum = 15;

class GameModel
{
public:
    GameModel();

public:
    std::vector<std::vector<int>> gameMapVec; // �洢��ǰ��Ϸ���̺����ӵ����,�հ�Ϊ0������1������-1
    std::vector<std::vector<int>> scoreMapVec; // �洢������λ�������������ΪAI��������
    bool playerFlag; // ��ʾ���巽
    GameType gameType; // ��Ϸģʽ
    GameStatus gameStatus; // ��Ϸ״̬

    void startGame(GameType type); // ��ʼ��Ϸ
    void calculateScore(); // ��������
    void actionByPerson(int row, int col); // ��ִ������
    void actionByAI(int& clickRow, int& clickCol); // ����ִ������
    void updateGameMap(int row, int col); // ÿ�����Ӻ������Ϸ����
    bool isWin(int row, int col); // �ж���Ϸ�Ƿ�ʤ��
    bool isDeadGame(); // �ж��Ƿ����
};

#endif // GAMEMODEL_H
