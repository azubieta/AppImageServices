#include "utils.h"

QString removeUriProtocolFromPath(const QString& uri) {
    if (uri.startsWith("file://"))
        return uri.mid(7);
    else
        return uri;
}
