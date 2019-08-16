#pragma once
// libraries
#include <QVariantMap>

class AbstractReader {
public:
    virtual QVariantMap readAppInfo() = 0;
};



