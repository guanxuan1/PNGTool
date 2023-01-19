#include "QtWidgets.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtWidgets w;
    w.show();
    return a.exec();
}
