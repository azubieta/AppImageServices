#pragma once
// system

//libraries

//local
#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtCore/QXmlStreamAttributes>
#include "AbstractReader.h"


class AppStreamReader : public AbstractReader {
public:
    explicit AppStreamReader(const QByteArray& appStreamData);

    QVariantMap readAppInfo() override;

private:
    QVariantMap appInfo;
    QVariantMap appInfoLinks;
    QByteArray appStreamData;
    QXmlStreamReader appstream;

    void readAppStream();

    void readAppStreamComponent();

    void readAppStreamComponentId();

    void readAppStreamComponentProjectLicense();

    void readAppStreamComponentName();

    void readAppStreamComponentSummary();

    void readAppStreamComponentUrl();

    void readAppStreamComponentReleases();

    void consumeElement();
};



