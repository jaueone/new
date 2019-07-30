#include "download.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Download w;
    w.show();
    if(w.is_res == false)
    {
        w.hide();
    }
    return a.exec();
}
