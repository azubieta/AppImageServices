// libraries
#include <QDBusConnection>
#include <QDebug>

// local
#include "UpdaterDBusInterface.h"
#include "UpdaterTaskDBusInterface.h"
#include "utils.h"

// generated by `qt5_add_dbus_adaptor` cmake function
#include "updateradaptor.h"

UpdaterDBusInterface::UpdaterDBusInterface(QObject* parent) : QObject(parent) {
    new UpdaterAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    bool operationSucceed;
    operationSucceed = dbus.registerObject(UPDATER_DBUS_OBJECT_PATH, this);
    if (!operationSucceed)
        qCritical() << "Unable to register d-bus object: " << UPDATER_DBUS_OBJECT_PATH;

    operationSucceed = dbus.registerService(UPDATER_DBUS_INTERFACE_NAME);
    if (!operationSucceed)
        qCritical() << "Unable to register d-bus service: " << UPDATER_DBUS_INTERFACE_NAME;

    operationSucceed = dbus.registerService(UPDATER_TASK_DBUS_INTERFACE_NAME);
    if (!operationSucceed)
        qCritical() << "Unable to register d-bus service: " << UPDATER_TASK_DBUS_INTERFACE_NAME;

    connect(&updaterService, &UpdaterService::taskStarted, this, &UpdaterDBusInterface::onTaskStarted);
    connect(&updaterService, &UpdaterService::taskFinished, this, &UpdaterDBusInterface::onTaskFinished);
}

UpdaterDBusInterface::~UpdaterDBusInterface() = default;

QString UpdaterDBusInterface::update(const QString& appImagePath) {
    const QString path = removeUriProtocolFromPath(appImagePath);

    return updaterService.update(path);
}

void UpdaterDBusInterface::onTaskStarted(const QString& taskId) {
    qDebug() << "Started: " << taskId;
    std::shared_ptr<appimage::update::Updater> task = updaterService.getTask(taskId);
    if (task) {
        // Publish Task object
        new UpdaterTaskDBusInterface(taskId, task, this);

        // Forward signal
        emit taskStarted(taskId);
    }
}

void UpdaterDBusInterface::onTaskFinished(const QString& taskId, bool result) {
    qDebug() << "Finished: " << taskId << " result: " << result;

    // Forward signal
    emit taskFinished(taskId, result);
}