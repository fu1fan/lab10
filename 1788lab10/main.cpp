#include "lab10.h"
#include "common.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    lab10 w;
    w.show();
    return a.exec();
}
