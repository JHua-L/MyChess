#ifndef CHESS_H
#define CHESS_H
#include <QMainWindow>
#include<QPainter>
#include<QRect>
#include<QString>
#include<QPen>
#include<QColor>
#include<Qt>
#include<QResizeEvent>
#include<QMouseEvent>

#include <QWidget>

class Chess : public QWidget
{
    Q_OBJECT

public:
    Chess(QWidget *parent = 0);
    ~Chess();
    //棋子类型
    enum ChessType{Empty=0,White,Black};

    //外界调用，改变背景图片
    void setBGImage(const QString fileName);
    //改变线的属性
    void setLine(const QColor color,const Qt::PenStyle style,const int width);
    //改变棋盘的显示值
    void setChessStatus(void *p);


protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    QString bg_fileName;
    QColor lineColor;
    Qt::PenStyle lineStyle;
    int lineWidth;
    //格子宽高，划线起始点
    int gridWidth,gridHeight,startX,startY;

    //保存棋子数据的容器
    int chessData[8][8];

    //初始化函数
    void Init();
    //初始化棋盘
    void InitChess();
signals:
    //发送点击的坐标
    void signalSendChessData(int i,int j);
};

#endif // CHESS_H
