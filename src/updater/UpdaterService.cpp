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
    tasks.insert(task->getId(), task);

    connect(task, &UpdaterTaskDBusInterface::stateChanged, this, &UpdaterService::onTaskStateChanged);

    return task->getId();
}

UpdaterService::~UpdaterService() {
    // Stop all tasks
    for (const auto& task: tasks)
        task->cancel();
}


void UpdaterService::onTaskStateChanged(int state) {
    UpdaterTaskDBusInterface* task = dynamic_cast<UpdaterTaskDBusInterface*>(sender());

    if (task != nullptr) {
        if (state == UpdaterTaskDBusInterface::Running)
            emit taskStarted(task->getId());

        if (state == UpdaterTaskDBusInterface::Finished) {
            emit taskFinished(task->getId(), true);


            tasks.remove(task->getId());
            task->deleteLater();
        }


        if (state == UpdaterTaskDBusInterface::Canceled || state == UpdaterTaskDBusInterface::Faulty) {
            emit taskFinished(task->getId(), false);

            tasks.remove(task->getId());
            task->deleteLater();
        }
    }
}

void UpdaterService::check(const QString& path) {
}
