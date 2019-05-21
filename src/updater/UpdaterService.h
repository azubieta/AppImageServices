#pragma once
// system
#include <memory>

// libraries
#include <QMap>
#include <QTimer>
#include <appimage/update.h>

class UpdaterService : public QObject {
Q_OBJECT

public:
    explicit UpdaterService(QObject* parent = nullptr);

    QString update(const QString& path);

    std::shared_ptr<appimage::update::Updater> getTask(const QString& taskId);

    ~UpdaterService() override;

signals:

    void taskStarted(QString taskId);

    void taskFinished(QString taskId, bool result);

protected slots:

    void updateTasks();

private:
    QMap<QString, std::shared_ptr<appimage::update::Updater>> tasks;
    QTimer updatesClock;
};
