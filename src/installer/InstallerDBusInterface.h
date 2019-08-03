#pragma once

// libraries
#include <QtCore/QObject>
#include <QtCore/QProcess>

// local
#include "InstallerDefines.h"

class InstallerDBusInterface : public QObject {
Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", INSTALLER_DBUS_INTERFACE_NAME)
public:
    explicit InstallerDBusInterface(QObject *parent = nullptr);

public Q_SLOTS:
    bool install(QString appImagePath);

    bool uninstall(QString appImagePath);

private:
    bool handleSubprocessResult(const QProcess &process) const;
};