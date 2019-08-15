// libraries
#include <QtCore/QDebug>

// local
#include <iostream>
#include "AppStreamReader.h"

AppStreamReader::AppStreamReader(const QByteArray& appStreamData) : appStreamData(appStreamData) {}

QVariantMap AppStreamReader::readAppInfo() {
    appstream.addData(appStreamData);
    readAppStream();

    appInfo["links"] = appInfoLinks;

    return appInfo;
}

void AppStreamReader::readAppStream() {
    while (!appstream.atEnd()) {
        appstream.readNext();

        if (appstream.isStartElement())
            if (appstream.name() == "component")
                readAppStreamComponent();
//            qDebug() << "+ " << appstream.name().toString();
//        if (appstream.isEndElement())
//            qDebug() << "- " << appstream.name().toString();
    }
    if (appstream.hasError()) {
        std::cerr << "Error while parsing the AppStream: \"" << appstream.errorString().toStdString() << "\" at line: "
                  << appstream.lineNumber() << std::endl;
    }
}

void AppStreamReader::readAppStreamComponent() {
    while (!appstream.atEnd() && !appstream.isEndElement()) {
        appstream.readNext();

        if (appstream.isStartElement()) {
            const QString name = appstream.name().toString();
            bool consumed = false;

            if (name == "id") {
                readAppStreamComponentId();
                consumed = true;
            }

            if (!consumed && name == "project_license") {
                readAppStreamComponentProjectLicense();
                consumed = true;
            }

            if (!consumed && name == "name") {
                readAppStreamComponentName();
                consumed = true;
            }

            if (!consumed && name == "summary") {
                readAppStreamComponentSummary();
                consumed = true;
            }

            if (!consumed && name == "releases") {
                readAppStreamComponentReleases();
                consumed = true;
            }

            if (!consumed && name == "url") {
                readAppStreamComponentUrl();
                consumed = true;
            }

            if (!consumed)
                consumeElement();

            // consume element end
            appstream.readNext();
        }
    }
}

void AppStreamReader::readAppStreamComponentId() {
    // consume element begin
    appstream.readNext();

    QString id = appstream.text().toString();
    appInfo["id"] = id.chopped(8);

    appstream.readNext();
}

void AppStreamReader::readAppStreamComponentProjectLicense() {
    // consume element begin
    appstream.readNext();

    appInfo["license"] = appstream.text().toString();

    appstream.readNext();
}

void AppStreamReader::readAppStreamComponentName() {
    QXmlStreamAttributes attributes = appstream.attributes();
    QString lang = attributes.value("xml:lang").toString();

    // consume element begin
    appstream.readNext();

    // ignore localized values
    if (lang.isEmpty())
        appInfo["name"] = appstream.text().toString();

    appstream.readNext();
}

void AppStreamReader::readAppStreamComponentSummary() {
    QXmlStreamAttributes attributes = appstream.attributes();
    QString lang = attributes.value("xml:lang").toString();

    // consume element begin
    appstream.readNext();

    // ignore localized values
    if (lang.isEmpty())
        appInfo["summary"] = appstream.text().toString();

    appstream.readNext();
}

void AppStreamReader::readAppStreamComponentUrl() {
    QXmlStreamAttributes attributes = appstream.attributes();
    QString type = attributes.value("type").toString();

    // consume element begin
    appstream.readNext();

    QString url = appstream.text().toString();

    appInfoLinks[type] = url;

    appstream.readNext();
}

void AppStreamReader::readAppStreamComponentReleases() {
    QString version;

    while (!appstream.atEnd() && !appstream.isEndElement()) {
        appstream.readNext();

        if (appstream.isStartElement()) {
            const QString name = appstream.name().toString();

            if (name == "release") {
                QXmlStreamAttributes attributes = appstream.attributes();
                QString newVersion = attributes.value("version").toString();

                // Assume the latest release as the current
                if (newVersion > version)
                    version = newVersion;
            }
            consumeElement();

            appstream.readNext();
        }
    }

    if (!version.isEmpty())
        appInfo["version"] = version;
}

void AppStreamReader::consumeElement() {
    int depth = 1;
    while (!appstream.atEnd() && depth > 0) {
        appstream.readNext();

        if (appstream.isStartElement())
            depth++;

        if (appstream.isEndElement())
            depth--;
    }
}
