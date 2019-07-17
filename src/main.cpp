// libraries
#include <QCoreApplication>

#include <appimage/appimage++.h>
#include <QtCore/QSettings>

// local
#include "LauncherDBusInterface.h"
#include "UpdaterDBusInterface.h"
#include "InspectorDBusInterface.h"
#include "ThumbnailerDBusInterface.h"

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("AppImage Services");
    QCoreApplication::setOrganizationDomain("org.appimage");
    QCoreApplication::setOrganizationName("The AppImage Project");

    InspectorDBusInterface inspectorDBusInterface(&app);
    LauncherDBusInterface launcherDBusInterface(&app);
    UpdaterDBusInterface updaterDBusInterface(&app);
    ThumbnailerDBusInterface thumbnailerDBusInterface(&app);

    return QCoreApplication::exec();
}
