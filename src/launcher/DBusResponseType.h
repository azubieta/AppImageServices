#pragma ONCE

#include <QMetaType>

enum class DBusResponseType {
    SUCCEED = 0,
    FAILED,
    FILE_NOT_FOUND,
    NOT_ENOUGH_PERMISSIONS
};

Q_DECLARE_METATYPE(DBusResponseType)
