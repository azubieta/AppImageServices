// libraries
#include <QDebug>
#include <QFile>
#include <QStandardPaths>

// local
#include "utils.h"

QString removeUriProtocolFromPath(const QString& uri) {
    if (uri.startsWith("file://"))
        return uri.mid(7);
    else
        return uri;
}

QString settingsFilePath() {
    static QString fileName = "/org.appimage.services.conf";
    QStringList configDirs = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);
    for (const QString& dir: configDirs) {
        QString path = dir + fileName;
        if (QFile::exists(path)) {
            qDebug() << "Using config file: " << path;
            return path;
        }

    }

    return configDirs.first() + fileName;
}
