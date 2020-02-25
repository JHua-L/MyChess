#include "chessform.h"
#include "ui_chessform.h"
#include<QDebug>
#include<QMessageBox>

#define GRID_NUMBER 8
int Twenty = 20;

ChessForm::ChessForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChessForm)
{
    ui->setupUi(this);
    Init();
    timerInit();
}

ChessForm::~ChessForm()
{
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------functions-----------

void ChessForm::Init(){
    QStringList array;
    array.append("白子先");
     array.append("黑子先");
    ui->cbox_item->addItems(array);

    roleInit("../myChess/image/white.png","../myChess/image/black.png");

    currentRole = Chess::Empty;

    //申请棋盘类空间
    myChess = new Chess();

    connect(myChess,SIGNAL(signalSendChessData(int,int)),this,SLOT(doProcessChessData(int,int)));

    //把棋盘显示在当前容器中
    ui->gridLayout->addWidget(myChess);

}

void ChessForm::roleInit(const QString whiteFileName, const QString blackFileName){
    ui->lbl1->setPixmap(QPixmap(whiteFileName));
    ui->lbl1->setScaledContents(true);

    ui->lbl2->setPixmap(QPixmap(blackFileName));
    ui->lbl2->setScaledContents(true);
}

void ChessForm::setRole(Chess::ChessType role){
    currentRole = role;
    if(role==Chess::Black){
        ui->lbl2->setVisible(true);
        ui->lbl1->setVisible(false);
    }else{
        ui->lbl1->setVisible(true);
        ui->lbl2->setVisible(false);
    }
    ui->lcdNumber1->display(0);
    ui->lcdNumber2->display(0);
}

int ChessForm::judgeRole(int x, int y,void *chess,Chess::ChessType currentRole, bool eatChess){
    //落子的八个方向：x=-1表示左方，y=-1表示上方，x、y均为-1则是左上方，依次类推
        int dir[8][2] = {{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
        int tmpX = x,tmpY = y;              //临时保存棋盘坐标位置（如上面分析中的A位置）
        int i = 0, eatNum = 0;      //初始化数据，eatNum用来统计可吃子的个数，不能吃则最终返回0
        typedef int (*p)[8];

        p chessStatus = p(chess);

        if(chessStatus[tmpX][tmpY] != Chess::Empty){//如果此方格中已经有棋子无效操作，直接返回
            return 0;
        }

        //棋盘的8个方向
        for(i=0; i<8; i++){
            //从鼠标点击点开始
            tmpX = x;
            tmpY = y;
            //确定一个方向：dir[i][0]
            tmpX += dir[i][0];
            tmpY += dir[i][1];

            if(tmpX < GRID_NUMBER && tmpX >= 0 && tmpY < GRID_NUMBER && tmpY >= 0
                && (chessStatus[tmpX][tmpY] != currentRole) && (chessStatus[tmpX][tmpY] != Chess::Empty)){

                //如果没有出界并且相邻棋子是对方棋子才有可能吃子
                tmpX += dir[i][0];
                tmpY += dir[i][1];//向前走一步，开始判断该方向还有无自己棋子

                while(tmpX < GRID_NUMBER && tmpX >= 0 && tmpY < GRID_NUMBER && tmpY >= 0){
                    if(chessStatus[tmpX][tmpY] == Chess::Empty){
                        break;//遇到空位跳出循环，外部if最后一个条件也会不满足，则直接判断下一个方向
                    }
                    if(chessStatus[tmpX][tmpY] == currentRole){//找到自己的棋子代表可以吃子
                        //能吃子则点击点标记为自己的棋子，update后是自己的棋子，否则点击处不能落子
                        (true == eatChess) ? (chessStatus[x][y] = currentRole) : true;//为false则不修改状态
                        tmpX -= dir[i][0];
                        tmpY -= dir[i][1];//回退一步开始吃子
                        //没有回到开始的位置就继续执行
                        while((tmpX != x) || (tmpY != y)){//没有回退到点击处则继续修改原有棋子状态
                            //若为true则为落子，修改为自己的棋子,如果为false则为测试，不用修改
                            (true == eatChess) ? (chessStatus[tmpX][tmpY] = currentRole) : true;
                            tmpX -= dir[i][0];
                            tmpY -= dir[i][1];//回退一格，继续
                            eatNum++;   //吃子累计
                        }
                        break;//跳出循环，结束该方向的判断
                    }
                    //没找到自己的棋子就向前（指定方向）走一步，走到0或GRID_NUMBER边界条件时就结束该层if语句
                    tmpX += dir[i][0];
                    tmpY += dir[i][1];
                }
            }
        }
        return eatNum;
}

void ChessForm::setChessInit(){
    //memset(formChessData,0,sizeof(int)*64);
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            formChessData[i][j]=Chess::Empty;
        }
    }
    formChessData[3][3] = Chess::Black;
    formChessData[3][4] = Chess::White;
    formChessData[4][3] = Chess::White;
    formChessData[4][4] = Chess::Black;
    myChess->setChessStatus(formChessData);

    ui->lcdNumber1->display(2);
    ui->lcdNumber2->display(2);
}

void ChessForm::roleChange(){
    if(Twenty==0&&Currentpk==PVC){
        if(currentRole==Chess::White){
            currentRole = Chess::Black;
        }else{
            currentRole = Chess::White;
        }
        timer->stop();
        timer->start();
        //机器下子
        robotRole(currentRole);
        chessShow();
        qDebug()<<Twenty<<"$$"<<endl;
    }else{
        Twenty=20;
        if(currentRole==Chess::White){
            currentRole = Chess::Black;
        }else{
            currentRole = Chess::White;
        }
        if(currentRole==Chess::Black){
            ui->lbl2->setVisible(true);
            ui->lbl1->setVisible(false);
            ui->lcd_blackLeftTime->setVisible(true);
            ui->lcd_whiteLeftTime->setVisible(false);
            ui->lbl_b->setVisible(true);
            ui->lbl_w->setVisible(false);
        }else{
            ui->lbl1->setVisible(true);
            ui->lbl2->setVisible(false);
            ui->lcd_blackLeftTime->setVisible(false);
            ui->lcd_whiteLeftTime->setVisible(true);
            ui->lbl_b->setVisible(false);
            ui->lbl_w->setVisible(true);
        }
    }

}

void ChessForm::chessShow(){
    int blackCount=0,whiteCount=0;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(formChessData[i][j]==Chess::White){
                whiteCount++;
            }else if(formChessData[i][j]==Chess::Black){
                blackCount++;
            }
        }
    }
    ui->lcdNumber1->display(whiteCount);
    ui->lcdNumber2->display(blackCount);
    ui->lcd_whiteLeftTime->display(Twenty);
    ui->lcd_blackLeftTime->display(Twenty);
    int totle = whiteCount + blackCount;
    if(whiteCount==0||(totle==64&&blackCount>whiteCount)){
        timer->stop();
        cal_time->stop();
        QString digTitle = "游戏结束";
        QString str = "黑子获胜！！";
        QMessageBox::information(this,digTitle,str,QMessageBox::Ok,QMessageBox::NoButton);
        setChessInit();
    }else if(blackCount==0||(totle==64&&blackCount<whiteCount)){
        timer->stop();
        cal_time->stop();
        QString digTitle = "游戏结束";
        QString str = "白子获胜！！";
        QMessageBox::information(this,digTitle,str,QMessageBox::Ok,QMessageBox::NoButton);
        setChessInit();
    }
}

void ChessForm::robotRole(Chess::ChessType role){
    Twenty = 20;
    int flag = 0;//0不能下子 1可以下子
    int ret,oldret=-1;
    int c_i,c_j;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            //遍历看能否下子
            if(formChessData[i][j]==Chess::Empty){
                ret = judgeRole(i,j,formChessData,currentRole,false);
                if(ret>oldret){
                    flag++;
                    c_i = i;
                    c_j = j;
                    oldret = ret;
                }
            }

        }
    }
    if(flag){
        //下子
        judgeRole(c_i,c_j,formChessData,currentRole,true);
        myChess->setChessStatus(formChessData);
        //切换界面显示
         roleChange();

    }else{
        roleChange();
    }
}

void ChessForm::timerInit(){
    timer = new QTimer(this);
    timer->stop();
    timer->setInterval(20000);
    connect(timer,SIGNAL(timeout()),this,SLOT(do_timeOut()));

    cal_time = new QTimer(this);
    cal_time->stop();
    cal_time->setInterval(1000);
    connect(cal_time,SIGNAL(timeout()),this,SLOT(do_cai_time()));
}

void ChessForm::do_timeOut(){
    roleChange();
}

void ChessForm::do_cai_time(){
    if(Twenty>0){
        Twenty--;
    }
    chessShow();
}

//-----------------------------------------------------------------------------------------------------------------end function-------


//------------------------------------------------------------------------------------------------------------------events-----------------
void ChessForm::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.drawPixmap(this->rect(),QPixmap("../myChess/image/BG_2.png"));
}
//------------------------------------------------------------------------------------------------------------------end event--------------



//------------------------------------------------------------------------------------------------------------------slots-----------------
void ChessForm::on_btn_pvp_clicked()
{
    Currentpk = PVP;
    //把界面初始化
    if(ui->cbox_item->currentText().contains("白")){
        setRole(Chess::White);
    }else{
        setRole(Chess::Black);
    }
    //把棋盘初始化
    setChessInit();
    timer->start();
    cal_time->start();
}

void ChessForm::doProcessChessData(int i, int j){
    //判断能否下子
    int ret = judgeRole(i,j,formChessData,currentRole,true);
    if(ret){
        //把数据传送给棋盘类
        myChess->setChessStatus(formChessData);
        if(Currentpk == PVP){
            //角色转换
            roleChange();
            timer->stop();
            timer->start();
            //数据统计
            chessShow();
        }else if(Currentpk == PVC){


            //角色转换
            roleChange();
            timer->stop();
            timer->start();
            //机器下子
            robotRole(currentRole);
            //数据统计
            chessShow();

        }

    }
}

void ChessForm::on_btn_pvc_clicked()
{

    Currentpk = PVC;
    //把界面初始化
    if(ui->cbox_item->currentText().contains("白")){
        setRole(Chess::White);
    }else{
        setRole(Chess::Black);
    }
    if(currentRole==Chess::White){
        ui->btn_blackHandOut->setVisible(false);
        ui->btn_blackGameOver->setVisible(false);
    }else{
        ui->btn_whiteHandOut->setVisible(false);
        ui->btn_whiteGameOver->setVisible(false);
    }


    //把棋盘初始化
    setChessInit();
    timer->start();
    cal_time->start();
}

void ChessForm::on_btn_whiteHandOut_clicked()
{
    if(Currentpk==PVC){
        Twenty = 0;
    }
    roleChange();
}

void ChessForm::on_btn_blackHandOut_clicked()
{
    if(Currentpk==PVC){
        Twenty = 0;
    }
    roleChange();
}

void ChessForm::on_btn_whiteGameOver_clicked()
{
    QString digTitle = "游戏结束";
    QString str = "黑子获胜！！";
    QMessageBox::information(this,digTitle,str,QMessageBox::Ok,QMessageBox::NoButton);
    timer->stop();
    setChessInit();
}

void ChessForm::on_btn_blackGameOver_clicked()
{
    QString digTitle = "游戏结束";
    QString str = "白子获胜！！";
    QMessageBox::information(this,digTitle,str,QMessageBox::Ok,QMessageBox::NoButton);
    timer->stop();
    setChessInit();
}
//------------------------------------------------------------------------------------------------------------------end slot--------------






