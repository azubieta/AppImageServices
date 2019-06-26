// libraries
#include <QCoreApplication>

#include <appimage/appimage++.h>

// local
#include "LauncherDBusInterface.h"
#include "UpdaterDBusInterface.h"
#include "InspectorDBusInterface.h"

int main(int argc, char** argv) {
    QCoreApplication a(argc, argv);

    InspectorDBusInterface inspectorDBusInterface(&a);
    LauncherDBusInterface launcherDBusInterface(&a);
    UpdaterDBusInterface updaterDBusInterface(&a);
    return a.exec();
}
