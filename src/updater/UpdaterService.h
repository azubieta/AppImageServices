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

private:

    QMap<QString, UpdaterTaskDBusInterface*> tasks;
};
