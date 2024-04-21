#include "QSQLite3MultipleCiphersDemo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSQLite3MultipleCiphersDemo w;
    w.show();
    return a.exec();
}
