
### 同济软件工程数据结构期末项目——五子棋
---
#### 下载链接
<a href="./Gobang.exe" target="_blank">五子棋 V1.0</a>

---
### 开发背景

由于五子棋游戏的对弈过程具有较大的分支因素，对计算机的性能要求较高。因此，找出一种五子棋游戏必胜的策略，在计算资源有限的短时间内是件非常困难的事情。本文提出了基于蒙特卡洛树的五子棋算法模型，在计算资源有限的的条件下，寻找五子棋棋局中的最优落子方案。

### 相关理论

蒙特卡洛树搜索分为四个阶段：

1. 选择阶段：搜索树以当前棋面状态为根节点开始，通过递归地应用子选择策略遍历树，直到到达一个还不属于搜索树的位置，即未被扩展过的节点。被选择的子节点表示在当前棋面状态下的最优选择。

2. 扩展阶段：根据可选择的落子操作，将节点添加到搜索树中来展开树。

3. 模拟阶段：在新扩展的节点上搜索树根据默认策略运行模拟对局，并产生游戏结果。

4. 反向传播：根据模拟阶段产生的游戏结果通过所选择的叶子节点通过反向传播传播的方式更新这个叶子节点的统计信息。


![image](https://github.com/Zhang-Shi-Team/Gobang/blob/master/Picture/MCTS1.png)

![image](https://github.com/Zhang-Shi-Team/Gobang/blob/master/Picture/MCTS2.png)

### 关键代码设计

##### MCTS类关键设计

```c++
/// @brief 选择
std::pair<Chess, int> treePolicy(Chess chess, std::pair<int, int> center, int nowblack);

/// @brief 扩展
/// @attention 目前只有5*5的范围内搜索,5*5外有更优解则取更优解
Chess expandNode(Chess chess, std::pair<int, int> center, int nowblack);


/// @brief 模拟    游戏至结束，黑色赢返回1，白色返回-1，和棋返回0
void defaultPolicy(Chess chess, int nowblack, int& value);

/// @brief 回退
void backUp(Chess x, Chess y, int value);
```

##### 多线程扩展模拟叶节点

```c++
 ThreadPool pool(threadNum);

 for (int i = 1; i <= simulationNum; i++)
     pool.enqueue(Confun, this, selectPoint.first, selectPoint.second ^ 1, chess);
```





### 开发人员 

|  姓名  |
| :----: |
| 张靖凯 |
| 时天逸 |


### 环境依赖
- Qt Creator 4.11.1(不建议通过此版本creator打开此项目，因为此版本creator不支持部分C++语法)
- VS 2019 / VS 2022 (建议用VS打开)

### 运行截图

![image](https://github.com/Zhang-Shi-Team/Gobang/blob/master/Picture/runPicture.png)
