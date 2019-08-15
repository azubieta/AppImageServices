#pragma once

// libraries
#include <QString>
#include <QXmlStreamReader>
#include <appimage/appimage++.h>
#include <XdgUtils/DesktopEntry/DesktopEntry.h>

// local
#include "AppStreamReader.h"

class AppImageInfoReader : public AbstractReader {

public:
    explicit AppImageInfoReader(const QString& appImagePath);

    QVariantMap readAppInfo() override;

private:
    appimage::core::AppImage appImage;

    bool hasDesktopEntry;
    QString desktopEntryFile;
    QByteArray desktopEntryData;
    bool hasAppStream;
    QByteArray appStreamData;

    // File extraction

    void loadDataFiles();

    bool isMainDesktopFile(const QString& fileName) const;

    bool isAppStreamFile(const QString& fileName) const;

    // Desktop Entry

    const char* const idKeyPath = "Desktop Entry/X-AppId";

    // Appstream

    QByteArray readFileData(appimage::core::PayloadIterator& itr) const;
};
