#pragma once

// libraries
#include <QObject>
#include <QDBusMessage>
#include <QDBusAbstractAdaptor>
#include <QJsonDocument>

// local
#include "AppImageInfoReader.h"
#include "ApplicationInfo.h"
#include "InspectorDefines.h"

class InspectorDBusInterface : public QObject {
Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", INSPECTOR_DBUS_INTERFACE_NAME)
public:
    explicit InspectorDBusInterface(QObject* parent = nullptr);

    ~InspectorDBusInterface() override;

public Q_SLOTS:

    /**
     * Read the application information from `.desktop` and `appstream.data.xml`
     * @param appImagePath
     * @return JSON formatted representation of an ApplicationInfo object
     */
    QString getApplicationInfo(QString appImagePath);

    QStringList listContents(const QString& appImagePath);

};

