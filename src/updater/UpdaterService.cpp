// libraries
#include <QString>
#include <QObject>
#include <QUuid>
#include <QDebug>

// local
#include "UpdaterService.h"


UpdaterService::UpdaterService(QObject* parent) : QObject(parent) {
    connect(&updatesClock, SIGNAL(timeout()), this, SLOT(updateTasks()));
}

QString UpdaterService::update(const QString& path) {
    auto taskPtr = std::make_shared<appimage::update::Updater>(path.toStdString());
    taskPtr->start();

    QString taskId = QUuid::createUuid().toString(QUuid::Id128);
    tasks.insert(taskId, taskPtr);

    QMetaObject::invokeMethod(this, &UpdaterService::updateTasks);
    emit taskStarted(taskId);

    return taskId;
}

UpdaterService::~UpdaterService() {
    // Stop all tasks
    for (const auto& task: tasks)
        task->stop();
}

void UpdaterService::updateTasks() {
    if (!tasks.empty()) {
        updatesClock.start(200);

        qDebug() << "Tasks progress: ";
        QStringList tasksToRemove;
        for (QMap<QString, std::shared_ptr<appimage::update::Updater>>::iterator itr = tasks.begin();
             itr != tasks.end(); ++itr) {
            if (itr.value()->isDone())
                tasksToRemove << itr.key();
        }


        for (const auto& taskId: tasksToRemove) {
            auto task = tasks[taskId];
            emit taskFinished(taskId, task->state() == appimage::update::Updater::SUCCESS);
            tasks.remove(taskId);
        }
    } else
        updatesClock.stop();
}

std::shared_ptr<appimage::update::Updater> UpdaterService::getTask(const QString& taskId) {
    return tasks.value(taskId, std::shared_ptr<appimage::update::Updater>());
}
