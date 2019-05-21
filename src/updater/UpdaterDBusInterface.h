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

protected slots:

    void onTaskStarted(const QString& taskId);

    void onTaskFinished(const QString& taskId, bool result);

signals:

    void taskStarted(QString taskId);

    void taskFinished(QString taskId, bool result);

private:
    UpdaterService updaterService;
};
