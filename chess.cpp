#include "chess.h"
#include<QDebug>
#include<QRgb>

#define GRIDCOUNT 10;
Chess::Chess(QWidget *parent)
    : QWidget(parent)
{
    Init();
    //InitChess();
}

Chess::~Chess()
{

}


//--------------events-------------------
void Chess::paintEvent(QPaintEvent *){
    QPainter painter(this);
    //画背景
    QRect rec(QPoint(0,0),QPoint(this->width(),this->height()));
    QPixmap pix(bg_fileName);
    painter.drawPixmap(rec,pix);

    //画线
    QPen pen1;
    pen1.setColor(lineColor);
    pen1.setStyle(lineStyle);
    pen1.setWidth(lineWidth);
    painter.setPen(pen1);

    for(int i=0;i<=8;i++){
        //x
        painter.drawLine(startX,startY + i*gridHeight,9*gridWidth,startY+i*gridHeight);
        //y
        painter.drawLine(startX+i*gridWidth,startY,startX+i*gridWidth,9*gridHeight);
    }

    //画棋子
    QString chessFileName;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(chessData[i][j]==White){
                chessFileName = "../myChess/image/white.png";
            }else if(chessData[i][j]==Black){
                chessFileName = "../myChess/image/black.png";
            }else{
                chessFileName.clear();
                continue;
            }
            painter.drawPixmap(startX+i*gridWidth,startY+j*gridHeight,gridWidth,gridHeight,QPixmap(chessFileName));
        }
    }

}

void Chess::resizeEvent(QResizeEvent *event){
    gridWidth = event->size().width()/GRIDCOUNT;
    gridHeight = event->size().height()/GRIDCOUNT;
    startX = gridWidth;
    startY = gridHeight;
}

void Chess::mousePressEvent(QMouseEvent *event){
    int x = event->x();
    int y = event->y();
    //x
    if(x>=startX&&(x<=startX+8*gridWidth)){
       //y
        if(y>=startY&&(y<=startY+8*gridHeight)){
            //得出当前坐标属于哪个格子
            int i=0,j=0;
            i=(x-startX)/gridWidth;
            j=(y-startY)/gridHeight;
//            qDebug()<<"i:"<<i<<"j:"<<j;
//            chessData[i][j]= Black;
//            this->update();
            signalSendChessData(i,j);
        }
    }
}

//--------------end event-------------------

//--------------functions-------------------

//private

//初始化函数
void Chess::Init(){
    bg_fileName.clear();
    bg_fileName = "../myChess/image/BGImage.png";

    lineColor = QColor(255, 75, 78);
    lineStyle = Qt::SolidLine;
    lineWidth = 3;



}
//初始化棋盘数据
void Chess::InitChess(){
    //memset(chessData,0,sizeof(int)*64);
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            chessData[i][j]=Empty;
        }
    }
//    chessData[3][3] = Black;
//    chessData[3][4] = White;
//    chessData[4][3] = White;
//    chessData[4][4] = Black;
}

//public
void Chess::setBGImage(const QString fileName){
    this->bg_fileName = fileName;
    this->update();
}
void Chess::setLine(const QColor color, const Qt::PenStyle style, const int width){
    this->lineColor = color;
    this->lineStyle = style;
    this->lineWidth = width;
    this->update();
}
void Chess::setChessStatus(void *p){
    memcpy(chessData,p,sizeof(int)*8*8);
    this->update();
}

//--------------end function-------------------




//--------------slots-------------------
//--------------end slot-------------------
