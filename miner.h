#ifndef MINER_H
#define MINER_H

#include <QWidget>




//地雷类

class miner : public QWidget
{
    Q_OBJECT
public:

    miner();//miner类的构造函数

    //地图信息
    int Row;
    int Col;
    int miner_num;

    void Create();//创建地图Map
    void Create(int row,int col,int miner_num); //用户自定义游戏数据 创建扫雷地图
    void ReSetMiner();//往地图里埋地雷
    bool Mouse_L_Click(int x,int y);
    bool Mouse_LR_Click(int x,int y);
    bool Mouse_R_Click(int x,int y);
    bool Win();
    int around_flag(int x,int y);


    int ** Map;//二维数组 用来存储地雷与其他正常方块

    bool first_time=true;
    int remain_miner=0;
    int remain_flag;
    int remain_time;
    //0游戏失败 1游戏进行中 2游戏胜利
    //为什么用0 1 2作为胜利判断 是因为后面作图的时候需要用到这个变量
    int winflag=1;
    //是否打开此处砖块
    bool isOpen=false;

    //explicit miner_window(QWidget *parent = nullptr);

signals:


};

#endif // MINER_H
