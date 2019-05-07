// libraries
#include <QCoreApplication>

// local
#include "launcher/DBusInterface.h"

int main(int argc, char** argv) {
    QCoreApplication a(argc, argv);

    DBusInterface interface(&a);
    return a.exec();
}
