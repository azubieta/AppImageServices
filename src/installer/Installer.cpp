// system
#include <utility>

// libraries
#include <QtCore/QDir>

// local
#include "Installer.h"

Installer::Installer(const QString &installPrefix) : installDir(installPrefix),
                                                     integrationManager(installDir.absoluteFilePath(dataDirPath).toStdString()) {

}

void Installer::install(const QString &appImagePath) {
    QDir binDir = createBinDir();

    QString targetBinaryPath = installBinary(appImagePath, binDir);

    registerApplication(targetBinaryPath);
}

void Installer::registerApplication(const QString &targetBinaryPath) const {
    try {
        appimage::core::AppImage appImage(targetBinaryPath.toStdString());
        integrationManager.registerAppImage(appImage);
    } catch (const appimage::core::AppImageError &error) {
        throw InstallerError(error.what());
    }
}

const QString Installer::installBinary(const QString &appImagePath, const QDir &binDir) const {
    QFileInfo appImageFileInfo(appImagePath);
    QFile appImageFile(appImagePath);
    QString targetFilePath = binDir.absoluteFilePath(appImageFileInfo.fileName());
    bool succeed = appImageFile.copy(targetFilePath);
    if (!succeed)
        throw InstallerError("Unable to copy binary to: " + targetFilePath);

    return targetFilePath;
}

QDir Installer::createBinDir() const {
    QString absoluteBinPath = installDir.filePath(binDirPath);
    bool succeed = installDir.mkpath(absoluteBinPath);
    if (!succeed)
        throw InstallerError("Unable to create bin dir: " + absoluteBinPath);

    return QDir(absoluteBinPath);
}

QString Installer::getInstallPrefix() const {
    return installDir.path();
}

const QString &Installer::getBinDir() const {
    return binDirPath;
}

