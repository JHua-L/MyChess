#include"chessform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Chess w;
//    w.show();
    ChessForm w;
    w.show();

    return a.exec();
}
