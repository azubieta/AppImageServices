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

    /**
     * List files included in the AppImage.
     * @param appImagePath
     * @return
     */
    QStringList listContents(const QString& appImagePath);

    /**
     * Extract a file from <path> to the <target>
     * @param appImagePath
     * @param source relative path to the file inside the AppImage
     * @param target path were the file will be extracted
     * @return
     */
    bool extractFile(QString appImagePath, QString source, QString target);
};

