#pragma once

// libraries
#include <QObject>
#include <QDBusMessage>
#include <QDBusAbstractAdaptor>

// local
#include "DBusResponseType.h"
#include "LauncherService.h"
#include "LauncherDefines.h"

class LauncherDBusInterface : public QObject {
Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", LAUNCHER_DBUS_INTERFACE_NAME)

public:
    explicit LauncherDBusInterface(QObject* parent = nullptr);

    ~LauncherDBusInterface() override;

public Q_SLOTS:
    /**
     * Starts the AppImage pointed by <appImagePath> using a self built runtime.
     * @param appImagePath
     */
    bool launch(const QString& appImagePath, const QStringList& args) const;

    /**
     * Add a launcher entry to the application pointed by <appImagePath> and updates the desktop environment cache.
     *
     * @param appImagePath
     * @return
     */
    bool registerApp(const QString& appImagePath) const;

    /**
     * Remove any launcher entry related to <appImagePath> and updates the desktop environment cache.
     *
     * @param appImagePath
     * @return
     */
    bool unregisterApp(const QString& appImagePath) const;

private:
    /*
     * Remove the Uri Protocol 'file://' from <path> if exists
     */
    static QString removeUriProtocolFromPath(const QString &path);

    LauncherService launcherService;
};

