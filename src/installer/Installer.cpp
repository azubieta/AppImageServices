// system
#include <fstream>
#include <utility>

// libraries
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QDebug>
#include <appimage/appimage.h>
#include <appimage/utils/ResourcesExtractor.h>
#include <XdgUtils/DesktopEntry/DesktopEntry.h>
#include <XdgUtils/DesktopEntry/DesktopEntryStringsValue.h>

// local
#include "Installer.h"
#include "utils.h"

Installer::Installer(const QString& installPrefix) : installDir(installPrefix),
                                                     integrationManager(
                                                             installDir.absoluteFilePath(dataDirPath).toStdString()) {

}

void Installer::install(const QString& appImagePath) {
    QDir binDir = createBinDir();

    QString targetBinaryPath = installBinary(appImagePath, binDir);

    registerApplication(targetBinaryPath);
}

void Installer::registerApplication(const QString& targetBinaryPath) const {
    try {
        appimage::core::AppImage appImage(targetBinaryPath.toStdString());
        if (integrationManager.shallAppImageBeRegistered(appImage)) {
            integrationManager.registerAppImage(appImage);
            addUninstallDesktopEntryAction(targetBinaryPath.toStdString());
        }
    } catch (const appimage::core::AppImageError& error) {
        throw InstallerError(error.what());
    }
}

QString Installer::installBinary(const QString& appImagePath, const QDir& binDir) const {
    QFileInfo appImageFileInfo(appImagePath);
    QFile appImageFile(appImagePath);
    QString targetFilePath = binDir.absoluteFilePath(appImageFileInfo.fileName());
    if (QFile::exists(targetFilePath))
        throw InstallerError("Application already installed: " + targetFilePath);

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

const QString& Installer::getBinDir() const {
    return binDirPath;
}

void Installer::addUninstallDesktopEntryAction(const std::string& appImagePath) const {
    using namespace XdgUtils::DesktopEntry;
    using namespace appimage::core;
    using namespace appimage::utils;

    try {
        // Guess the target desktop file path
        QDir desktopEntriesDir(installDir.absoluteFilePath(dataDirPath) + "/applications/");
        char* pathMd5 = appimage_get_md5(appImagePath.c_str());
        QString nameFilter = QString("appimagekit_%1-*").arg(pathMd5);
        QStringList entries = desktopEntriesDir.entryList({nameFilter});

        QString targetDesktopFilePath;
        if (!entries.empty())
            targetDesktopFilePath = desktopEntriesDir.absoluteFilePath(entries.first());

        if (QFile::exists(targetDesktopFilePath)) {
            QSettings settings(settingsFilePath(), QSettings::IniFormat);

            std::ifstream ifstream(targetDesktopFilePath.toStdString());
            DesktopEntry entry(ifstream);

            std::string actionsString = static_cast<std::string>(entry["Desktop Entry/Actions"]);
            DesktopEntryStringsValue actions(actionsString);

            // Add uninstall action
            actions.append("Uninstall");
            entry.set("Desktop Entry/Actions", actions.dump());

            entry.set("Desktop Action Uninstall/Name", "Uninstall");
            entry.set("Desktop Action Uninstall/Icon", "uninstall");

            // Build Remove Action Exec Value
            QString uninstallCommand = settings.value("Launcher/UninstallCommand", "").toString();
            QString uninstallCommandArgs = settings.value("Launcher/UninstallCommandArgs", "").toString();

            qDebug() << settings.fileName();
            qDebug() << settings.allKeys();
            qDebug() << "settings uninstall command " << uninstallCommand;
            qDebug() << "settings uninstall command args" << uninstallCommandArgs;
            // Use direct dbus calls as fallback method
            if (uninstallCommand.isEmpty()) {
                uninstallCommand = "pkexec appimage-services";
                uninstallCommandArgs = QString("uninstall %1").arg(QString::fromStdString(appImagePath));
            } else {
                // replace appimage path argument
                uninstallCommandArgs = uninstallCommandArgs.arg(QString::fromStdString(appImagePath));
            }

            entry.set("Desktop Action Uninstall/Exec", (uninstallCommand + " " + uninstallCommandArgs).toStdString());
            entry.set("Desktop Action Uninstall/TryExec", uninstallCommand.toStdString());

            std::ofstream ofstream(targetDesktopFilePath.toStdString());
            ofstream << entry;
        } else {
            qWarning() << "Unable to add uninstall action. Generated desktop file not found at: "
                       << targetDesktopFilePath;
        }
    } catch (const AppImageError& error) {
        qWarning() << "Unable to add uninstall action. " << error.what();
    }

}

void Installer::uninstall(const QString& appImagePath) {
    try {
        appimage::core::AppImage appImage(appImagePath.toStdString());
        bool succeed = QFile::remove(appImagePath);
        if (!succeed)
            throw InstallerError("Unable to uninstall application " + appImagePath + ". Unable to remove file.");

        unregisterApplication(appImagePath);
    } catch (const appimage::core::AppImageError& error) {
        throw InstallerError("Unable to uninstall application " + appImagePath + ". " + error.what());
    }
}

void Installer::unregisterApplication(const QString& appImagePath) const {
    try {
        integrationManager.unregisterAppImage(appImagePath.toStdString());
    } catch (const appimage::core::AppImageError& error) {
        throw InstallerError(error.what());
    }
}

