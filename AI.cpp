#include "AI.h"




//////////////////////////////////////////////////////
////              AI Object
//////////////////////////////////////////////////////
//void AI::getAINextStep()
//{
//    count = 0;
//    qDebug() << "started at:" << QTime::currentTime().toString();
//    qDebug() << "SCORE:" << alpha_beta(parentWindow->gomoku, 0, -99999999, 9999999);
//    qDebug() << count << " chesses searched";
//    qDebug() << "ended at:" << QTime::currentTime().toString();
//    emit stepReady();
//}
//int AI::getChessScore(int chess[15][15], int leftEdge, int rightEdge, int upEdge, int downEdge)
//{
//    int whitePatternNum[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//    int blackPatternNum[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//    int score = 0;
//    //先分析行-
//    for (int i = upEdge; i <= downEdge; ++i)
//    {
//        QString line;
//        for (int j = leftEdge; j <= rightEdge; ++j)
//        {
//            line += QString::number(chess[j][i]);
//        }
//        //      if(debug)qDebug()<<"-:"<<line<<endl;
//        score += getLineScore(line, whitePatternNum, blackPatternNum);
//    }
//    //分析列|
//    for (int i = leftEdge; i <= rightEdge; ++i)
//    {
//        QString line;
//        for (int j = upEdge; j <= downEdge; ++j)
//        {
//            line += QString::number(chess[i][j]);
//        }
//        //      if(debug)qDebug()<<"|:"<<line<<endl;
//        score += getLineScore(line, whitePatternNum, blackPatternNum);
//    }
//    //分析斜向下\
//    //下半部分
//    for (int i = upEdge; i <= downEdge; ++i)
//    {
//        QString line;
//        for (int j = 0; (i + j) <= downEdge && (j + leftEdge) <= rightEdge; ++j)
//        {
//            line += QString::number(chess[j + leftEdge][i + j]);
//        }
//        if (line.size() < 5)
//            break;
//        //      if(debug)qDebug()<<"\\ down:"<<line<<endl;
//        score += getLineScore(line, whitePatternNum, blackPatternNum);
//    }
//    //上半部分
//    for (int i = leftEdge + 1; i <= rightEdge; ++i)
//    {
//        QString line;
//        for (int j = 0; (i + j) <= rightEdge && (upEdge + j) <= downEdge; ++j)
//        {
//            line += QString::number(chess[i + j][j + upEdge]);
//        }
//        if (line.size() < 5)
//            break;
//        //      if(debug)qDebug()<<"\\ up:"<<line<<endl;
//        score += getLineScore(line, whitePatternNum, blackPatternNum);
//    }

//    //分析斜向上/
//    //上半部分
//    for (int i = downEdge; i >= upEdge; --i)
//    {
//        QString line;
//        for (int j = 0; (leftEdge + j) <= rightEdge && (i - j) >= upEdge; ++j)
//        {
//            line += QString::number(chess[leftEdge + j][i - j]);
//        }
//        if (line.size() < 5)
//            break;
//        //      if(debug)qDebug()<<"/ up:"<<line<<endl;
//        score += getLineScore(line, whitePatternNum, blackPatternNum);
//    }

//    //下半部分
//    for (int i = leftEdge + 1; i <= rightEdge; ++i)
//    {
//        QString line;
//        for (int j = 0; (downEdge - j) >= upEdge && (i + j) <= rightEdge; ++j)
//        {
//            line += QString::number(chess[i + j][downEdge - j]);
//        }
//        if (line.size() < 5)
//            break;
//        //      if(debug)qDebug()<<"/ down:"<<line<<endl;
//        score += getLineScore(line, whitePatternNum, blackPatternNum);
//    }
//    //棋子的位置分计算
//    for (int i = leftEdge; i <= rightEdge; ++i)
//    {
//        for (int j = upEdge; j <= downEdge; ++j)
//        {
//            if (chess[i][j] == 0)
//                continue;
//            if (chess[i][j] == 1)
//            {
//                score += parentWindow->basicScore[i][j]; //电脑白子 加分
//                continue;
//            }
//            if (chess[i][j] == 2)
//            {
//                score -= parentWindow->basicScore[i][j]; //对手黑子 减分
//                continue;
//            }
//        }
//    }

//    //必杀棋
//    if (whitePatternNum[0] > 0 || blackPatternNum[0] > 0)
//    {
//        if (whitePatternNum[0] > blackPatternNum[0])
//            return 9999999;
//        else
//            return -9999999;
//    }
//    if (blackPatternNum[patternID::Active4] > 0)
//        return -8999999;
//    if (whitePatternNum[patternID::Active4] > 0)
//        return 8999999;

//    if (blackPatternNum[patternID::Active3] > 0)
//    {
//        //奇数是黑方准备落子,偶数是白方准备落子
//        //如果黑方有活三,且是奇数局面,黑方无敌
//        if (parentWindow->searchDepth % 2 == 1)
//            return -8999999;
//        else
//        {
//            if (whitePatternNum[patternID::Active3] >= blackPatternNum[patternID::Active3])
//                return 8999999;
//        }
//    }
//    if (whitePatternNum[patternID::Active3] > 1)
//    {
//        return -8999999;
//    }
//    if (whitePatternNum[patternID::Active3] > 0 && whitePatternNum[patternID::Sleep4] > 0)
//    {
//        if (!(blackPatternNum[patternID::Active3] > 0 && blackPatternNum[patternID::Sleep4] > 0))
//            return 8999999;
//        else
//            return -8999999;
//    }

//    return score;
//}
//int AI::getLineScore(QString line, int whitePatternNum[10], int blackPatternNum[15])
//{
//    int score = 0;
//    for (auto &p : parentWindow->patterns)
//    {
//        for (auto &wp : p.whitePattern)
//        { //电脑白子 加分
//            int c = line.count(wp);
//            score += p.whiteScore * c;
//            whitePatternNum[p.id] += c;
//        }
//        for (auto &bp : p.blackPattern)
//        { //对手黑子 减分
//            int c = line.count(bp);
//            score += p.blackScore * c;
//            blackPatternNum[p.id] += c;
//        }
//    }
//    return score;
//}

//std::vector<step> AI::getPossibleSteps(int chess[15][15], bool white, int leftEdge, int rightEdge, int upEdge, int downEdge)
//{
//    std::vector<step> possibleSteps;
//    for (int i = leftEdge; i <= rightEdge; ++i)
//    {
//        for (int j = upEdge; j <= downEdge; ++j)
//        {
//            if (chess[i][j] == 0)
//            {
//                possibleSteps.push_back(step(i, j, white));
//            }
//        }
//    }
//    return possibleSteps;
//}

////走法算分,用于排序
//bool AI::calculateScore(std::vector<step> &possibleSteps, int chess[15][15])
//{

//    for (auto &s : possibleSteps)
//    {
//        //记录各种模式的数量
//        int patternNum[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//        int score = 0;
//        //横向
//        QString lineWhite;
//        QString lineBlack;
//        for (int i = ((s.x - 5) >= 0 ? (s.x - 5) : 0); i <= ((s.x + 5) <= 14 ? (s.x + 5) : 14); ++i)
//        {
//            if (i != s.x)
//            {
//                lineWhite += QString::number(chess[i][s.y]);
//                lineBlack += QString::number(chess[i][s.y]);
//            }
//            else
//            {
//                lineWhite += QString::number(1);
//                lineBlack += QString::number(2);
//            }
//        }
//        //      qDebug()<<"-:"<<line<<endl;
//        for (auto &p : parentWindow->patterns)
//        {
//            for (auto &wp : p.whitePattern)
//            {
//                int c = lineWhite.count(wp);
//                score += p.whiteScore * c;
//                patternNum[p.id] += c;
//            }
//            for (auto &bp : p.blackPattern)
//            {
//                int c = lineBlack.count(bp);
//                score -= p.blackScore * c;
//                patternNum[p.id] += c;
//            }
//        }
//        //纵向
//        lineWhite.clear();
//        lineBlack.clear();
//        for (int i = ((s.y - 5) >= 0 ? (s.y - 5) : 0); i <= ((s.y + 5) <= 14 ? (s.y + 5) : 14); ++i)
//        {

//            if (i != s.y)
//            {
//                lineWhite += QString::number(chess[s.x][i]);
//                lineBlack += QString::number(chess[s.x][i]);
//            }
//            else
//            {
//                lineWhite += QString::number(1);
//                lineBlack += QString::number(2);
//            }
//        }
//        //      qDebug()<<"|:"<<line<<endl;
//        for (auto &p : parentWindow->patterns)
//        {
//            for (auto &wp : p.whitePattern)
//            {
//                int c = lineWhite.count(wp);
//                score += p.whiteScore * c;
//                patternNum[p.id] += c;
//            }
//            for (auto &bp : p.blackPattern)
//            {
//                int c = lineBlack.count(bp);
//                score -= p.blackScore * c;
//                patternNum[p.id] += c;
//            }
//        }
//        // \向
//        lineWhite.clear();
//        lineBlack.clear();
//        for (int i = -5; i <= 5; ++i)
//        {
//            if ((s.x + i) < 0 || (s.y + i) < 0)
//                continue;
//            if ((s.x + i) > 14 || (s.y + i) > 14)
//                break;
//            if (i != 0)
//            {
//                lineWhite += QString::number(chess[s.x + i][s.y + i]);
//                lineBlack += QString::number(chess[s.x + i][s.y + i]);
//            }
//            else
//            {
//                lineWhite += QString::number(1);
//                lineBlack += QString::number(2);
//            }
//        }
//        //      qDebug()<<"\\:"<<line<<endl;
//        for (auto &p : parentWindow->patterns)
//        {
//            for (auto &wp : p.whitePattern)
//            {
//                int c = lineWhite.count(wp);
//                score += p.whiteScore * c;
//                patternNum[p.id] += c;
//            }
//            for (auto &bp : p.blackPattern)
//            {
//                int c = lineBlack.count(bp);
//                score -= p.blackScore * c;
//                patternNum[p.id] += c;
//            }
//        }
//        //  /向
//        lineWhite.clear();
//        lineBlack.clear();
//        for (int i = -5; i <= 5; ++i)
//        {
//            if ((s.x + i) < 0 || (s.y - i) > 14)
//                continue;
//            if ((s.y - i) < 0 || (s.x + i) > 14)
//                break;

//            if (i != 0)
//            {
//                lineWhite += QString::number(chess[s.x + i][s.y - i]);
//                lineBlack += QString::number(chess[s.x + i][s.y - i]);
//            }
//            else
//            {
//                lineWhite += QString::number(1);
//                lineBlack += QString::number(2);
//            }
//        }
//        //      qDebug()<<"/:"<<line<<endl;
//        for (auto &p : parentWindow->patterns)
//        {
//            for (auto &wp : p.whitePattern)
//            {
//                int c = lineWhite.count(wp);
//                score += p.whiteScore * c;
//                patternNum[p.id] += c;
//            }
//            for (auto &bp : p.blackPattern)
//            {
//                int c = lineBlack.count(bp);
//                score -= p.blackScore * c;
//                patternNum[p.id] += c;
//            }
//        }

//        //算分
//        s.score = score;
//        //有必杀棋后面都不用算了
//        //      if(patternNum[patternID::LongLink]>0){//||patternNum[patternID::Active4]>0||patternNum[patternID::Active3]>2){
//        //          auto bestStep=s;
//        //          possibleSteps.clear();
//        //          possibleSteps.push_back(bestStep);
//        //          return true;
//        //          break;
//        //        }
//    }
//    return false;
//}
//void AI::deleteUselessStep(std::vector<step> &possibleSteps, int chess[15][15])
//{
//    for (auto ite = possibleSteps.begin(); ite != possibleSteps.end();)
//    {
//        int counter = 0;
//        chess[ite->x][ite->y] = 3; //测试用子,表示这里有一个子反正不是0
//        for (int i = ((ite->x - 2) > 0 ? (ite->x - 2) : 0); i <= ((ite->x + 2) < 14 ? (ite->x + 2) : 14); ++i)
//        {
//            for (int j = ((ite->y - 2) > 0 ? (ite->y - 2) : 0); j <= ((ite->y + 2) < 14 ? (ite->y + 2) : 14); ++j)
//            {
//                if (chess[i][j] != 0)
//                    ++counter;
//            }
//        }
//        chess[ite->x][ite->y] = 0;
//        if (counter > 1)
//            ++ite;
//        else
//        {
//            ite = possibleSteps.erase(ite);
//        }
//    }
//}

////集成算杀
//bool AI::reserveKillStep(std::vector<step> &possibleSteps, int chess[15][15])
//{

//    for (auto s = possibleSteps.begin(); s != possibleSteps.end();)
//    {
//        //先看看是不是太偏的废棋
//        int counter = 0;
//        chess[s->x][s->y] = 3; //测试用子,表示这里有一个子反正不是0
//        for (int i = ((s->x - 2) > 0 ? (s->x - 2) : 0); i <= ((s->x + 2) < 14 ? (s->x + 2) : 14); ++i)
//        {
//            for (int j = ((s->y - 2) > 0 ? (s->y - 2) : 0); j <= ((s->y + 2) < 14 ? (s->y + 2) : 14); ++j)
//            {
//                if (chess[i][j] != 0)
//                    ++counter;
//            }
//        }
//        chess[s->x][s->y] = 0;
//        if (counter <= 1)
//        {
//            s = possibleSteps.erase(s);
//            continue;
//        }

//        //记录各种模式的数量
//        int patternNum[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//        int score = 0;
//        //横向
//        QString lineWhite;
//        QString lineBlack;
//        for (int i = ((s->x - 5) >= 0 ? (s->x - 5) : 0); i <= ((s->x + 5) <= 14 ? (s->x + 5) : 14); ++i)
//        {
//            if (i != s->x)
//            {
//                lineWhite += QString::number(chess[i][s->y]);
//                lineBlack += QString::number(chess[i][s->y]);
//            }
//            else
//            {
//                lineWhite += QString::number(1);
//                lineBlack += QString::number(2);
//            }
//        }
//        //      qDebug()<<"-:"<<line<<endl;
//        for (auto &p : parentWindow->patterns)
//        {
//            for (auto &wp : p.whitePattern)
//            {
//                int c = lineWhite.count(wp);
//                score += p.whiteScore * c;
//                patternNum[p.id] += c;
//            }
//            for (auto &bp : p.blackPattern)
//            {
//                int c = lineBlack.count(bp);
//                score -= p.blackScore * c;
//                patternNum[p.id] += c;
//            }
//        }
//        //纵向
//        lineWhite.clear();
//        lineBlack.clear();
//        for (int i = ((s->y - 5) >= 0 ? (s->y - 5) : 0); i <= ((s->y + 5) <= 14 ? (s->y + 5) : 14); ++i)
//        {

//            if (i != s->y)
//            {
//                lineWhite += QString::number(chess[s->x][i]);
//                lineBlack += QString::number(chess[s->x][i]);
//            }
//            else
//            {
//                lineWhite += QString::number(1);
//                lineBlack += QString::number(2);
//            }
//        }
//        //      qDebug()<<"|:"<<line<<endl;
//        for (auto &p : parentWindow->patterns)
//        {
//            for (auto &wp : p.whitePattern)
//            {
//                int c = lineWhite.count(wp);
//                score += p.whiteScore * c;
//                patternNum[p.id] += c;
//            }
//            for (auto &bp : p.blackPattern)
//            {
//                int c = lineBlack.count(bp);
//                score -= p.blackScore * c;
//                patternNum[p.id] += c;
//            }
//        }
//        // \向
//        lineWhite.clear();
//        lineBlack.clear();
//        for (int i = -5; i <= 5; ++i)
//        {
//            if ((s->x + i) < 0 || (s->y + i) < 0)
//                continue;
//            if ((s->x + i) > 14 || (s->y + i) > 14)
//                break;
//            if (i != 0)
//            {
//                lineWhite += QString::number(chess[s->x + i][s->y + i]);
//                lineBlack += QString::number(chess[s->x + i][s->y + i]);
//            }
//            else
//            {
//                lineWhite += QString::number(1);
//                lineBlack += QString::number(2);
//            }
//        }
//        //      qDebug()<<"\\:"<<line<<endl;
//        for (auto &p : parentWindow->patterns)
//        {
//            for (auto &wp : p.whitePattern)
//            {
//                int c = lineWhite.count(wp);
//                score += p.whiteScore * c;
//                patternNum[p.id] += c;
//            }
//            for (auto &bp : p.blackPattern)
//            {
//                int c = lineBlack.count(bp);
//                score -= p.blackScore * c;
//                patternNum[p.id] += c;
//            }
//        }
//        //  /向
//        lineWhite.clear();
//        lineBlack.clear();
//        for (int i = -5; i <= 5; ++i)
//        {
//            if ((s->x + i) < 0 || (s->y - i) > 14)
//                continue;
//            if ((s->y - i) < 0 || (s->x + i) > 14)
//                break;

//            if (i != 0)
//            {
//                lineWhite += QString::number(chess[s->x + i][s->y - i]);
//                lineBlack += QString::number(chess[s->x + i][s->y - i]);
//            }
//            else
//            {
//                lineWhite += QString::number(1);
//                lineBlack += QString::number(2);
//            }
//        }
//        //      qDebug()<<"/:"<<line<<endl;
//        for (auto &p : parentWindow->patterns)
//        {
//            for (auto &wp : p.whitePattern)
//            {
//                int c = lineWhite.count(wp);
//                score += p.whiteScore * c;
//                patternNum[p.id] += c;
//            }
//            for (auto &bp : p.blackPattern)
//            {
//                int c = lineBlack.count(bp);
//                score -= p.blackScore * c;
//                patternNum[p.id] += c;
//            }
//        }

//        //算分
//        s->score = score;
//        //有必杀棋后面都不用算了
//        if (patternNum[patternID::LongLink] > 0)
//        { //||patternNum[patternID::Active4]>0||patternNum[patternID::Active3]>2){
//            auto bestStep = *s;
//            possibleSteps.clear();
//            possibleSteps.push_back(bestStep);
//            //          qDebug()<<"found a best step"<<endl;
//            return true;
//            break;
//        }
//        //开始算杀
//        if (patternNum[patternID::Active3] >= 1 || patternNum[patternID::Sleep4] >= 1)
//        {
//            ++s;
//        }
//        else
//        {
//            //没有活3和眠4的删除
//            s = possibleSteps.erase(s);
//            continue;
//        }
//    }
//    return false;
//}
//int AI::alpha_beta(int chess[15][15], int depth, int alpha, int beta)
//{
//    if (depth == parentWindow->searchDepth)
//    {

//        ++count; //计算总步数
//        //没有缓存的版本
//        //      int s=getChessScore(chess);
//        //      return s;

//        auto p = parentWindow->scoreCache.find(parentWindow->zobristHash);
//        if (p == parentWindow->scoreCache.end())
//        {
//            int s = getChessScore(chess);
//            parentWindow->scoreCache[parentWindow->zobristHash] = s;
//            return s;
//        }
//        else
//        {
//            return p->second;
//        }
//    }
//    std::vector<step> possibleSteps;
//    int score = 0;
//    //获得所有走法
//    if (depth % 2 == 0)
//    { //最大层
//        if (parentWindow->stepAlreadyMade > 22)
//            possibleSteps = getPossibleSteps(chess, true);
//        else if (parentWindow->stepAlreadyMade > 10)
//        {
//            possibleSteps = getPossibleSteps(chess, true,
//                                             3 - (parentWindow->stepAlreadyMade - 10) / 4,
//                                             11 + (parentWindow->stepAlreadyMade - 10) / 4,
//                                             3 - (parentWindow->stepAlreadyMade - 10) / 4,
//                                             11 + (parentWindow->stepAlreadyMade - 10) / 4);
//        }
//        else
//        {
//            possibleSteps = getPossibleSteps(chess, true,
//                                             6 - parentWindow->stepAlreadyMade / 2.5,
//                                             8 + parentWindow->stepAlreadyMade / 2.5,
//                                             6 - parentWindow->stepAlreadyMade / 2.5,
//                                             8 + parentWindow->stepAlreadyMade / 2.5);
//        }

//        //对走法进行排序
//        //新排序法
//        //      deleteUselessStep(possibleSteps,parentWindow->gomoku);
//        calculateScore(possibleSteps, parentWindow->gomoku);
//        //      if(calculateScore(possibleSteps,parentWindow->gomoku)&&depth==0){
//        //          parentWindow->AINextX=possibleSteps[0].x;
//        //          parentWindow->AINextY=possibleSteps[0].y;
//        //          return 9999999;
//        //          }
//        std::sort(possibleSteps.begin(), possibleSteps.end(), [](const step &s1, const step &s2) -> bool
//                  { return s1.score > s2.score; });

//        if (possibleSteps.size() > 10)
//        { //删除分数较低的节点试试看
//            possibleSteps.erase(possibleSteps.begin() + 10, possibleSteps.end());
//        }

//        //      if(depth<=parentWindow->searchDepth){
//        //      if(possibleSteps.size()>10){//删除分数较低的节点试试看
//        //          possibleSteps.erase(possibleSteps.begin()+10,possibleSteps.end());
//        //        }
//        //        }else{
//        //          if(possibleSteps.size()>7){//删除分数较低的节点试试看
//        //              possibleSteps.erase(possibleSteps.begin()+7,possibleSteps.end());
//        //            }
//        //        }

//        int count = 0;
//        for (auto &s : possibleSteps)
//        {
//            ++count;
//            //          if(depth==0)qDebug()<<"now searching step "<<++count<<endl;
//            chess[s.x][s.y] = 1; //走子
//            parentWindow->zobristHash ^= parentWindow->zobristWhite[s.x][s.y];
//            ++parentWindow->stepAlreadyMade;
//            parentWindow->XStack[parentWindow->stepAlreadyMade] = s.x;
//            parentWindow->YStack[parentWindow->stepAlreadyMade] = s.y;
//            score = alpha_beta(chess, depth + 1, alpha, beta);
//            chess[s.x][s.y] = 0; //恢复棋盘
//            parentWindow->zobristHash ^= parentWindow->zobristWhite[s.x][s.y];
//            --parentWindow->stepAlreadyMade;

//            if (score > alpha)
//            {
//                alpha = score;
//                if (depth == 0)
//                {
//                    parentWindow->AINextX = s.x;
//                    parentWindow->AINextY = s.y;
//                }
//            }
//            if (alpha >= beta)
//            {
//                return alpha;
//            }
//        }
//    }
//    else
//    { //最小层
//        if (parentWindow->stepAlreadyMade > 22)
//            possibleSteps = getPossibleSteps(chess, false);
//        else if (parentWindow->stepAlreadyMade > 10)
//        {
//            possibleSteps = getPossibleSteps(chess, true,
//                                             3 - (parentWindow->stepAlreadyMade - 10) / 4,
//                                             11 + (parentWindow->stepAlreadyMade - 10) / 4,
//                                             3 - (parentWindow->stepAlreadyMade - 10) / 4,
//                                             11 + (parentWindow->stepAlreadyMade - 10) / 4);
//        }
//        else
//        {
//            possibleSteps = getPossibleSteps(chess, false,
//                                             6 - parentWindow->stepAlreadyMade / 2.5,
//                                             8 + parentWindow->stepAlreadyMade / 2.5,
//                                             6 - parentWindow->stepAlreadyMade / 2.5,
//                                             8 + parentWindow->stepAlreadyMade / 2.5);
//        }

//        //对走法进行排序

//        //新排序法
//        //      deleteUselessStep(possibleSteps,parentWindow->gomoku);
//        calculateScore(possibleSteps, parentWindow->gomoku);

//        std::sort(possibleSteps.begin(), possibleSteps.end(), [](const step &s1, const step &s2) -> bool
//                  { return s1.score > s2.score; });

//        if (possibleSteps.size() > 10)
//        { //删除分数较低的节点试试看
//            possibleSteps.erase(possibleSteps.begin() + 10, possibleSteps.end());
//        }

//        for (auto &s : possibleSteps)
//        {
//            chess[s.x][s.y] = 2; //走子
//            parentWindow->zobristHash ^= parentWindow->zobristBlack[s.x][s.y];
//            ++parentWindow->stepAlreadyMade;
//            parentWindow->XStack[parentWindow->stepAlreadyMade] = s.x;
//            parentWindow->YStack[parentWindow->stepAlreadyMade] = s.y;
//            score = alpha_beta(chess, depth + 1, alpha, beta);
//            chess[s.x][s.y] = 0; //恢复棋盘
//            parentWindow->zobristHash ^= parentWindow->zobristBlack[s.x][s.y];
//            --parentWindow->stepAlreadyMade;
//            if (score < beta)
//                beta = score;
//            if (alpha >= beta)
//            {
//                return beta;
//            }
//        }
//    }
//    return depth % 2 == 0 ? alpha : beta;
//}
