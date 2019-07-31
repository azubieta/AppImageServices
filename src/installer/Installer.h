#pragma once
// system
#include <memory>

// libraries
#include <QDir>
#include <QString>
#include <appimage/desktop_integration/IntegrationManager.h>

// local
#include "InstallerError.h"

class Installer {

public:
    explicit Installer(const QString &installPrefix);

    void install(const QString &appImagePath);

    QString getInstallPrefix() const;

    const QString &getBinDir() const;

    void uninstall(const QString &appImagePath);

protected:
    QDir createBinDir() const;

    const QString installBinary(const QString &appImagePath, const QDir &binDir) const;

    void registerApplication(const QString &targetBinaryPath) const;

    void addUninstallDesktopEntryAction(const std::string& appImagePath) const;
private:
    QDir installDir;
    QString binDirPath = "bin";
    QString dataDirPath = "share";

    appimage::desktop_integration::IntegrationManager integrationManager;

    void unregisterApplication(const QString &appImagePath) const;
};

