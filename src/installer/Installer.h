#pragma once

// libraries
#include <memory>
#include <QString>
#include <appimage/desktop_integration/IntegrationManager.h>

#include "InstallerError.h"

class Installer {

public:
    explicit Installer(const QString &installPrefix);

    void install(const QString &appImagePath);

    QString getInstallPrefix() const;

    const QString &getBinDir() const;

protected:
    QDir createBinDir() const;

    const QString installBinary(const QString &appImagePath, const QDir &binDir) const;

    void registerApplication(const QString &targetBinaryPath) const;

private:
    QDir installDir;
    QString binDirPath = "bin";
    QString dataDirPath = "share";

    appimage::desktop_integration::IntegrationManager integrationManager;
};

