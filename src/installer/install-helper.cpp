// libraries
#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>

// local
#include "Installer.h"

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("appimage-services-install-helper");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("AppImage Services Install helper");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("appImagePath", QCoreApplication::translate("main", "AppImage file to install."));

    parser.process(app);

    const QStringList args = parser.positionalArguments();
    if (args.size() < 1)
        parser.showHelp(1);
    else {
        Installer installer("/usr/local");
        for (const QString &appImagePath: args)
            installer.install(appImagePath);

        return  0;
    }
}