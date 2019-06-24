#pragma once

// libraries
#include <QString>
#include <QXmlStreamReader>
#include <appimage/appimage++.h>
#include <XdgUtils/DesktopEntry/DesktopEntry.h>

// local
#include "ApplicationInfo.h"


class AppImageInfoReader {

public:
    explicit AppImageInfoReader(const QString& appImagePath);

    ApplicationInfo read();

private:
    ApplicationInfo info;

    appimage::core::AppImage appImage;

    bool hasDesktopEntry;
    XdgUtils::DesktopEntry::DesktopEntry desktopEntry;
    bool hasAppStream;
    QXmlStreamReader appstream;

    // File extraction

    void loadDataFiles();

    bool isMainDesktopFile(const QString& fileName) const;

    QString extractIdFromDesktopFileName(QString& fileName) const;

    bool isAppStreamFile(const QString& fileName) const;

    // Desktop Entry

    void readDesktopEntry();

    const char* const idKeyPath = "Desktop Entry/X-AppId";

    QStringList readDesktopEntryStringList(const std::string& string);

    // Appstream

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
