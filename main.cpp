#include "download.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Download w;
    w.show();

    return a.exec();
}
