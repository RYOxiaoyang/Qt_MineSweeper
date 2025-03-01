#ifndef WIDGET_H
#define WIDGET_H
#include"miner.h"
#include <QWidget>
#include<QTimer>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void paintEvent(QPaintEvent *);//绘图事件
    void mousePressEvent(QMouseEvent *event);//鼠标点击事件
    int offsetx=5;//x偏移量
    int offsety=30;//y偏移量
    miner *mine=new miner();
    QTimer *runtime=new QTimer();

private slots:
    void add_sectime();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H

