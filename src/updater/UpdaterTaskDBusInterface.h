#pragma once
// system
#include <memory>

// libraries
#include <QTimer>
#include <QObject>
#include <AppImageUpdaterBridge>

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
    enum State {
        Ready = 0,
        Running,
        Finished,
        Canceled,
        Faulty,
    };

    explicit UpdaterTaskDBusInterface(const QString& appImagePath, bool onlyCheck = false, QObject* parent = nullptr);

    ~UpdaterTaskDBusInterface() override;

public:
    const QString& getId() const;

    int getProgressValue();

    int getProgressTotal();

    int getState();

    QString getStatusMessage();

public slots:

    void cancel();

Q_SIGNALS:

    void stateChanged(int state);

    void statusMessageChanged(const QString& state);

    void progressValueChanged(int value);

    void progressTotalChanged(int value, int total);

protected slots:

    void onStarted();

    void onCanceled();

    void onFinished(QJsonObject, QString);

    void onStatusChanged(short);

    void onError(short);

    void onProgress(int, qint64, qint64, double, const QString&);

    void onUpdateAvailable(bool, QJsonObject);

private:
    QString id;
    QString dbusObjectPath;
    int state;
    QString statusMessage;

    int progressValue;
    int progressTotal;

    AppImageUpdaterBridge::AppImageDeltaRevisioner deltaRevisioner;

private:
    void publish();

    void setState(State newState);

    void connectSignals() const;
};
