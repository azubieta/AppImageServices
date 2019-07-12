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
    Q_PROPERTY(int errorCode READ getErrorCode)

    Q_PROPERTY(qint64 bytesReceived READ getBytesReceived NOTIFY bytesReceivedChanged)
    Q_PROPERTY(qint64 bytesTotal READ getBytesTotal NOTIFY bytesTotalChanged)
public:
    enum State {
        // Initial state
        Ready = 0,
        // Inner states
        ReadingFile = 10,
        LookingForUpdates = 20,
        DownloadingUpdate = 30,
        // Final states
        Errored = -1,
        NoUpdateAvailable = 21,
        UpdateAvailable = 22,
        DownloadCompleted = 31,
        Canceled = 200,
    };

    explicit UpdaterTaskDBusInterface(const QString& appImagePath, bool checkOnly = false, QObject* parent = nullptr);

    ~UpdaterTaskDBusInterface() override;

public:
    const QString& getId() const;

    int getState();

    int getErrorCode();

    int getBytesReceived();

    int getBytesTotal();

public slots:

    void start();

    void cancel();

Q_SIGNALS:

    void stateChanged(int state);

    void bytesReceivedChanged(int value);

    void bytesTotalChanged(int total);

    void finished();

    void error(int code);

protected slots:

    void onStarted();

    void onCanceled();

    void onFinished(QJsonObject, QString);

    void onStatusChanged(short);

    void onError(short);

    void onProgress(int, qint64, qint64, double, const QString&);

    void onUpdateAvailable(bool, QJsonObject);

private:
    QString dbusObjectPath;

    QString id;
    int state;
    int errorCode;

    qint64 bytesReceived;
    qint64 bytesTotal;

    bool checkOnly;

    AppImageUpdaterBridge::AppImageDeltaRevisioner deltaRevisioner;

private:
    void publish();

    void setState(State newState);

    void connectSignals() const;
};
