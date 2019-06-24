#include <utility>

// libraries
#include <appimage/appimage++.h>
#include <XdgUtils/DesktopEntry/DesktopEntry.h>
#include <XdgUtils/DesktopEntry/DesktopEntryStringsValue.h>

// local
#include "AppImageInfoReader.h"

using namespace XdgUtils::DesktopEntry;
using namespace appimage::core;

AppImageInfoReader::AppImageInfoReader(const QString& appImagePath) : appImage(appImagePath.toStdString()),
                                                                      hasDesktopEntry(false), hasAppStream(false) {}

ApplicationInfo AppImageInfoReader::read() {
    loadDataFiles();

    if (hasDesktopEntry)
        readDesktopEntry();

    if (hasAppStream)
        readAppStream();

    return info;
}

void AppImageInfoReader::readDesktopEntry() {
    try {
        QString name = QString::fromStdString(static_cast<std::string>(desktopEntry["Desktop Entry/Name"]));
        QString summary = QString::fromStdString(static_cast<std::string>(desktopEntry["Desktop Entry/Comment"]));

        QStringList categories = readDesktopEntryStringList(desktopEntry.get("Desktop Entry/Categories", ""));
        QStringList mimeTypes = readDesktopEntryStringList(desktopEntry.get("Desktop Entry/MimeType", ""));

        info.setName(name);
        info.setSummary(summary);
        info.setCategories(categories);
        info.setMimeTypes(mimeTypes);
    } catch (const AppImageError& error) {
        std::cerr << "Error while accessing the AppImage " << error.what() << std::endl;
    } catch (const DesktopEntryError& error) {
        std::cerr << "Error while parsing the Desktop Entry " << error.what() << std::endl;
    }
}

QStringList AppImageInfoReader::readDesktopEntryStringList(const std::string& string) {
    DesktopEntryStringsValue stringsValue(string);

    QStringList list;
    for (int i = 0; i < stringsValue.size(); ++i)
        list.push_back(QString::fromStdString(stringsValue[i]));

    return list;
}

void AppImageInfoReader::loadDataFiles() {
    for (auto itr = appImage.files(); itr != itr.end(); ++itr) {
        auto fileName = QString::fromStdString(*itr);
        if (isMainDesktopFile(fileName)) {
            try {
                itr.read() >> desktopEntry;
                hasDesktopEntry = true;

                const QString appId = extractIdFromDesktopFileName(fileName);
                info.setId(appId);
            } catch (const XdgUtils::DesktopEntry::DesktopEntryError& error) {
                std::cerr << "Error while parsing the Desktop Entry " << error.what() << std::endl;
            }
        }

        if (isAppStreamFile(fileName)) {
            appstream.clear();

            std::string data(std::istreambuf_iterator<char>(itr.read()), {});
            appstream.addData(data.c_str());

            hasAppStream = true;
        }
    }
}

bool AppImageInfoReader::isMainDesktopFile(const QString& fileName) const {
    return !fileName.contains("/") and fileName.contains(".desktop", Qt::CaseInsensitive);
}

QString AppImageInfoReader::extractIdFromDesktopFileName(QString& fileName) const {
    return fileName.remove(".desktop", Qt::CaseInsensitive);
}

bool AppImageInfoReader::isAppStreamFile(const QString& fileName) const {
    return fileName.startsWith("usr/share/metainfo") && fileName.endsWith("appdata.xml");
}

void AppImageInfoReader::readAppStream() {
    while (!appstream.atEnd()) {
        appstream.readNext();

        if (appstream.isStartElement())
            if (appstream.name() == "component")
                readAppStreamComponent();
    }
    if (appstream.hasError()) {
        std::cerr << "Error while parsing the AppStream: \"" << appstream.errorString().toStdString() << "\" at line: "
                  << appstream.lineNumber() << std::endl;
    }
}

void AppImageInfoReader::readAppStreamComponent() {
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

void AppImageInfoReader::readAppStreamComponentId() {
    // consume element begin
    appstream.readNext();

    QString id = appstream.text().toString();
    info.setId(extractIdFromDesktopFileName(id));

    appstream.readNext();
}

void AppImageInfoReader::readAppStreamComponentProjectLicense() {
    // consume element begin
    appstream.readNext();

    QString projectLicense = appstream.text().toString();
    info.setLicense(projectLicense);

    appstream.readNext();
}

void AppImageInfoReader::readAppStreamComponentName() {
    // consume element begin
    appstream.readNext();

    QString name = appstream.text().toString();
    info.setName(name);

    appstream.readNext();
}

void AppImageInfoReader::readAppStreamComponentSummary() {
    // consume element begin
    appstream.readNext();

    QString summary = appstream.text().toString();
    info.setSummary(summary);

    appstream.readNext();
}

void AppImageInfoReader::readAppStreamComponentUrl() {
    QXmlStreamAttributes attributes = appstream.attributes();
    QString type = attributes.value("type").toString();

    // consume element begin
    appstream.readNext();

    QString url = appstream.text().toString();

    info.addLink(type, url);
    appstream.readNext();
}

void AppImageInfoReader::readAppStreamComponentReleases() {
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

                consumeElement();
            }
        }
    }

    info.setVersion(version);
}

void AppImageInfoReader::consumeElement() {
    int depth = 1;
    while (!appstream.atEnd() && depth > 0) {
        appstream.readNext();

        if (appstream.isStartElement())
            depth++;

        if (appstream.isEndElement())
            depth--;
    }
}
