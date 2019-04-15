#include "tcpserwer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TCPSerwer w;
    w.show();

    return a.exec();
}
