#pragma once

// library
#include <QObject>

// local
#include "ThumbnailerDefines.h"

class ThumbnailerDBusInterface : public QObject {
Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", THUMBNAILER_DBUS_INTERFACE_NAME)

public:
    explicit ThumbnailerDBusInterface(QObject* parent = nullptr);

    ~ThumbnailerDBusInterface() override;

public Q_SLOTS:

    bool create(QString appImagePath);

    bool destroy(QString appImagePath);

};
