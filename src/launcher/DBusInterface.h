#pragma once

// libraries
#include <QObject>
#include <QDBusMessage>
#include <QDBusAbstractAdaptor>

// local
#include "DBusResponseType.h"
#include "LauncherService.h"
#include "LauncherDefines.h"

class DBusInterface : public QObject {
Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", LAUNCHER_DBUS_INTERFACE_NAME)

public:
    explicit DBusInterface(QObject* parent = nullptr);

    ~DBusInterface() override;

public Q_SLOTS:

    /**
     * Add a launcher entry to the application pointed by <appImagePath> and updates the desktop environment cache.
     *
     * @param appImagePath
     * @return
     */
    bool registerApp(const QString& appImagePath);

    /**
     * Remove any launcher entry related to <appImagePath> and updates the desktop environment cache.
     *
     * @param appImagePath
     * @return
     */
    bool unregisterApp(const QString& appImagePath);

private:
    LauncherService launcherService;
};

