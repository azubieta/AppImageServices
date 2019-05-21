// libraries
#include <QCoreApplication>

// local
#include "LauncherDBusInterface.h"
#include "UpdaterDBusInterface.h"

int main(int argc, char** argv) {
    QCoreApplication a(argc, argv);

    LauncherDBusInterface launcherDBusInterface(&a);
    UpdaterDBusInterface updaterDBusInterface(&a);
    return a.exec();
}
