#pragma once
// system
#include <memory>

// libraries
#include <QMap>
#include <QTimer>
#include <AppImageUpdaterBridge>
#include "UpdaterTaskDBusInterface.h"

class UpdaterService : public QObject {
Q_OBJECT

public:
    explicit UpdaterService(QObject* parent = nullptr);

    ~UpdaterService() override;

public slots:

    QString update(const QString& path);

    QString check(const QString& path);

signals:

    void taskStarted(QString taskId);

    void taskFinished(QString taskId, bool result);

protected slots:

    void onTaskStateChanged(int state);

private:

    QMap<QString, UpdaterTaskDBusInterface*> tasks;
};
