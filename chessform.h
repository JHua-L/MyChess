#ifndef CHESSFORM_H
#define CHESSFORM_H

#include"chess.h"
#include <QWidget>
#include<QPainter>
#include<QPushButton>
#include<QTimer>
namespace Ui {
class ChessForm;
}

class ChessForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChessForm(QWidget *parent = 0);
    ~ChessForm();
protected:
    void paintEvent(QPaintEvent *);
private slots:
    void on_btn_pvp_clicked();

    void do_timeOut();
    void do_cai_time();

    //处理棋盘发送过来的信号
    void doProcessChessData(int i,int j);


    void on_btn_pvc_clicked();

    void on_btn_whiteHandOut_clicked();

    void on_btn_blackHandOut_clicked();

    void on_btn_whiteGameOver_clicked();

    void on_btn_blackGameOver_clicked();

private:
    Ui::ChessForm *ui;
    Chess *myChess;
    QTimer *timer,*cal_time;
    //当前谁在下子
    Chess::ChessType currentRole;
    int formChessData[8][8];
    enum PKType {PVP,PVC,NVN};
    PKType Currentpk;

    void Init();
    int judgeRole(int x, int y,void *chess,Chess::ChessType currentRole, bool eatChess);

    void roleInit(const QString white,const QString Black);
    //设置谁先下
    void setRole(Chess::ChessType currentRole);

    //对棋盘初始化
    void setChessInit();

    //角色切换
    void roleChange();

    //数据统计
    void chessShow();

    //机器下子
    void robotRole(Chess::ChessType role);

    //定时器初始化
    void timerInit();
};

#endif // CHESSFORM_H
