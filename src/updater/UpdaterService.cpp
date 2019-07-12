// libraries
#include <QString>
#include <QObject>
#include <QUuid>
#include <QDebug>

// local
#include "UpdaterService.h"


UpdaterService::UpdaterService(QObject* parent) : QObject(parent) {}

QString UpdaterService::update(const QString& path) {
    auto task = new UpdaterTaskDBusInterface(path);
    return task->getId();
}

UpdaterService::~UpdaterService() {
    // Stop all tasks
    for (const auto& task: tasks)
        task->cancel();
}

QString UpdaterService::check(const QString& path) {
    auto task = new UpdaterTaskDBusInterface(path, true);
    tasks.insert(task->getId(), task);
    
    return task->getId();
}
