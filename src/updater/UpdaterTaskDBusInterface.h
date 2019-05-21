#pragma once
// system
#include <memory>

// libraries
#include <QTimer>
#include <QObject>
#include <appimage/update.h>

// local
#include "UpdaterDefines.h"

class UpdaterTaskDBusInterface : public QObject {
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", UPDATER_TASK_DBUS_INTERFACE_NAME)
Q_PROPERTY(QString id READ getId)
Q_PROPERTY(int state READ getState NOTIFY stateChanged)
Q_PROPERTY(int progressValue READ getProgressValue NOTIFY progressValueChanged)
Q_PROPERTY(int progressTotal READ getProgressTotal NOTIFY progressTotalChanged)
Q_PROPERTY(QString statusMessage READ getStatusMessage NOTIFY statusMessageChanged)

public:
    UpdaterTaskDBusInterface(const QString& id, std::shared_ptr<appimage::update::Updater> updater, QObject* parent);

    const QString& getId() const;

    int getProgressValue();

    int getProgressTotal();

    int getState();

    QString getStatusMessage();

    ~UpdaterTaskDBusInterface() override;

Q_SIGNALS:

    void stateChanged(int state);

    void statusMessageChanged(const QString& state);

    void progressValueChanged(int value);

    void progressTotalChanged(int value, int total);

protected slots:
    void updateTaskData();

private:
    QString id;
    QString dbusObjectPath;
    int state;
    QString statusMessage;

    int progressValue{};
    int progressTotal{};

    QTimer updateTimer;
    std::shared_ptr<appimage::update::Updater> updater;
};
