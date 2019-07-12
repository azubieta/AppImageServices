#pragma once

// libraries
#include <QObject>

// local
#include "UpdaterDefines.h"
#include "UpdaterService.h"

class UpdaterDBusInterface : public QObject {
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", UPDATER_DBUS_INTERFACE_NAME)

public:
    explicit UpdaterDBusInterface(QObject* parent = nullptr);

    ~UpdaterDBusInterface() override;

public slots:

    /**
     * Will try to update the AppImage pointed by <appImagePath>.
     * @param appImagePath
     * @return taskId or an empty string in case of error
     */
    QString update(const QString& appImagePath);

    /**
     * Will check for updates for the AppImage pointed by <appImagePath>.
     * @param appImagePath
     * @return taskId or an empty string in case of error
     */
    QString check(const QString& appImagePath);

private:
    UpdaterService updaterService;
};
