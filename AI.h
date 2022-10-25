#ifndef AI_H
#define AI_H

//class AI : public QObject
//{
//    Q_OBJECT
//public:
//    AI();
//    AI(MainWindow *p) : parentWindow(p) {}
//    int count;

//private:
//    MainWindow *parentWindow;

//public slots:
//    void getAINextStep();

//public:
//    int getChessScore(int chess[15][15], int leftEdge = 0, int rightEdge = 14, int upEdge = 0, int downEdge = 14);
//    int getLineScore(QString, int whitePatternNum[10], int blackPatternNum[15]);
//    std::vector<step> getPossibleSteps(int chess[15][15], bool white, int leftEdge = 0, int rightEdge = 14, int upEdge = 0, int downEdge = 14);
//    int alpha_beta(int chess[15][15], int depth, int alpha, int beta);
//    bool calculateScore(std::vector<step> &possibleSteps, int chess[15][15]);
//    void deleteUselessStep(std::vector<step> &possibleSteps, int chess[15][15]);
//    bool reserveKillStep(std::vector<step> &possibleSteps, int chess[15][15]);
//signals:
//    void stepReady();
//};

#endif // AI_H
