#include "widget.h"
#include "ui_widget.h"
#include<QMenuBar>
#include<miner.h>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPainter>
#include<QTimer>

//扫雷的主界面
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("来扫雷吧");
    QMenuBar *Menu=new QMenuBar(this);
    QMenu *start =new QMenu("开始");
    QAction *new_game =new QAction("新游戏");
    QAction *easy =new QAction("简单");
    QAction *normal =new QAction("普通");
    QAction *hard =new QAction("困难");
    QAction *hell =new QAction("极难");
    QAction *leave =new QAction("退出");

    //创建初始游戏
    mine->Create();
    this->setFixedSize(mine->Row*20+offsetx*2,mine->Col*20+offsety+46);
    this->move(QPoint(450,200));

    connect(this->runtime,SIGNAL(timeout()),this,SLOT(add_sectime()));

    //为窗口创建菜单栏
    Menu->addMenu(start);
    start->addAction(new_game);
    start->addSeparator();
    start->addAction(easy);
    start->addSeparator();
    start->addAction(normal);
    start->addSeparator();
    start->addAction(hard);
    start->addSeparator();
    start->addAction(hell);
    start->addSeparator();
    start->addAction(leave);

    connect(new_game,&QAction::triggered,[=](){
        mine->ReSetMiner();
        update();
    });
    connect(easy,&QAction::triggered,[=](){
        mine->Create(9,9,10);
        this->setFixedSize(mine->Row*20+offsetx*2,mine->Col*20+offsety+46);
    });
    connect(normal,&QAction::triggered,[=](){
        mine->Create(16,16,50);
        this->setFixedSize(mine->Row*20+offsetx*2,mine->Col*20+offsety+46);
    });
    connect(hard,&QAction::triggered,[=](){
        mine->Create(25,25,150);
        this->setFixedSize(mine->Row*20+offsetx*2,mine->Col*20+offsety+46);
    });
    connect(hell,&QAction::triggered,[=](){
        mine->Create(30,30,200);
        this->setFixedSize(mine->Row*20+offsetx*2,mine->Col*20+offsety+46);
    });
    connect(leave,&QAction::triggered,[=](){
        this->close();
    });

}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap time(":/time.bmp");
    QPixmap emoji(":/emoji.bmp");
    QPixmap tiles(":/item2.bmp");
    QPixmap title(":/title.bmp");

    //绘制上面的框架
    painter.drawPixmap( 0+offsetx, 0+offsety, title, 0, 0, 70, 40);

    painter.drawPixmap(mine->Row * 10 - 20+offsetx, 0+offsety, title, 80, 0, 40, 40);
    painter.drawPixmap(mine->Row * 20 - 70+offsetx, 0+offsety, title, 130, 0, 70, 40);
    painter.drawPixmap(70+offsetx, 0+offsety, mine->Row * 10 - 90, 40, title, 70, 0, 10, 40);
    painter.drawPixmap(mine->Row * 10 + 20+offsetx, 0+offsety, mine->Row * 10 - 90, 40, title, 70, 0, 10, 40);
    painter.drawPixmap(mine->Row * 10 - 12+offsetx, 7+offsety, emoji, mine->winflag*24, 0, 24, 24);
    //绘制地图
    for(int i=0;i<mine->Row;i++){
        for(int j=0;j<mine->Col;j++){
            //绘制数字
            if(mine->Map[i][j]>=0&&mine->Map[i][j]<=8){
                //这里的参数依次是 绘制设备的起始x坐标 y坐标 绘制设备中要画的图片 绘制图片的起始x坐标 y坐标 绘制长度 绘制宽度
                painter.drawPixmap(i*20+offsetx,j*20+40+offsety,tiles,mine->Map[i][j]*20,0,20,20);
            }
            //绘制地雷
            if(mine->Map[i][j]==-1){
                painter.drawPixmap(i*20+offsetx,j*20+40+offsety,tiles,9*20,0,20,20);
            }
            //绘制旗子
            if(mine->Map[i][j]>40&mine->Map[i][j]<60){
                painter.drawPixmap(i*20+offsetx,j*20+40+offsety,tiles,11*20,0,20,20);
            }
            //绘制插过的旗子
            if(mine->Map[i][j]==-2){
                painter.drawPixmap(i*20+offsetx,j*20+40+offsety,tiles,12*20,0,20,20);
            }
            //绘制没有打开过的地板
            if(mine->Map[i][j]>90){
                painter.drawPixmap(i*20+offsetx,j*20+40+offsety,tiles,10*20,0,20,20);
            }
        }
    }
    int remain_Mine=mine->remain_miner;
    int remain_Time=mine->remain_time;

    //绘制剩余的雷数

    //不允许数量小于0
    if(remain_Mine<0){
        remain_Mine=0;
    }
    //将任何remain_Mine看为三位数后对其/100 这样在它为三位数时可以获得非0百位数，否则获得0
    painter.drawPixmap(6+offsetx, 5+offsety, time, remain_Mine / 100 * 20, 0, 20, 28);
    //如果的确是三位数 让它对100取余 从而获得十位与个位
    if(remain_Mine>=100){
        remain_Mine%=100;
    }
    painter.drawPixmap(26+offsetx, 5+offsety, time, remain_Mine / 10 * 20, 0, 20, 28);
    painter.drawPixmap(46+offsetx, 5+offsety, time, remain_Mine % 10 * 20, 0, 20, 28);

    //绘制时间

    //先判断>=1000情况 不满足则判断>=100情况 其余部分与上面remain_Main作用一致
        if(remain_Time>=1000)
            remain_Time%=1000;
        painter.drawPixmap(mine->Row*20 - 66+offsetx, 5+offsety, time, remain_Time / 100 * 20, 0, 20, 28);
        if(remain_Time>=100)
            remain_Time%=100;
        painter.drawPixmap(mine->Row*20 - 46+offsetx, 5+offsety, time, remain_Time / 10 * 20, 0, 20, 28);
        painter.drawPixmap(mine->Row*20 - 26+offsetx, 5+offsety, time, remain_Time % 10 * 20, 0, 20, 28);
}


void Widget::mousePressEvent(QMouseEvent *event){
    //posx与posy 用来保存点击后的坐标
    int posx=event->x()-offsetx;
    int posy=event->y()-offsety;
    //在item2文件中 一个方格的大小为20X20 x的作用是获取点击后的方格的Row值 y获得Col值
    int x=(posx)/20;
    int y=(posy)/20-2;
    //位运算符 左键点击与右键点击中有一个为1 则返回1
    if(event->buttons()==(Qt::LeftButton|Qt::RightButton)){
        //判断是不是左右键一起点击
        if(mine->Mouse_LR_Click(x,y)){
            //每一次点击事件都需要重绘窗口
            if(mine->winflag==0||mine->winflag==2){
                if(this->runtime->isActive()){
                    this->runtime->stop();
                }
            }
            update();
        }
    }
    //左键点击事件有两个 一是点击砖块 二是点击表情
    else if(event->button()==Qt::LeftButton){
        //判断点击的坐标是否在表情的范围内 这里的判断空间比表情图标的规格略大 方便用户点击重来
        //posy选取4<x<34范围的原因是 初始化的时候y的大小是24
        if(posx>mine->Row * 10 -15 && posx<mine->Row * 10 +15 && posy > 4 && posy<34){
            mine->ReSetMiner();
            update();
            return;
        }
        //如果点击砖块
        if(mine->Mouse_L_Click(x,y))
        {
            //如果计时器没被激活
            if(!(this->runtime->isActive())){
                this->runtime->start(1000);
            }
            update();

        }
        //判断游戏是否结束 以便停止计时器
        if(mine->winflag==0||mine->winflag==2){
            if(this->runtime->isActive()){
                this->runtime->stop();
            }
        }
    }
    //右键点击事件
    else if(event->button()==Qt::RightButton){
        if(mine->Mouse_R_Click(x,y)){
            update();
        }
    }

}


void Widget::add_sectime()
{
    this->mine->remain_time++;
    update();
}



