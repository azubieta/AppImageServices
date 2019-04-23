// libraries
#include <QCoreApplication>

// local
#include "DBusInterface.h"

int main(int argc, char** argv) {
    QCoreApplication a(argc, argv);

    DBusInterface interface(&a);
    return a.exec();
}
