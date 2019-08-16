#include <utility>
#include <sstream>

// libraries
#include <QtCore/QDebug>
#include <appimage/appimage++.h>
#include <XdgUtils/DesktopEntry/DesktopEntry.h>

// local
#include "AbstractReader.h"
#include "AppStreamReader.h"
#include "DesktopEntryReader.h"
#include "AppImageInfoReader.h"

using namespace XdgUtils::DesktopEntry;
using namespace appimage::core;

AppImageInfoReader::AppImageInfoReader(const QString& appImagePath) : appImage(appImagePath.toStdString()),
                                                                      hasDesktopEntry(false), hasAppStream(false) {}

void AppImageInfoReader::loadDataFiles() {
    for (auto itr = appImage.files(); itr != itr.end(); ++itr) {
        auto fileName = QString::fromStdString(*itr);
        if (isMainDesktopFile(fileName)) {
            try {
                desktopEntryFile = fileName;
                desktopEntryData = readFileData(itr);
                hasDesktopEntry = true;
            } catch (const XdgUtils::DesktopEntry::DesktopEntryError& error) {
                std::cerr << "Error while parsing the Desktop Entry " << error.what() << std::endl;
            }
        }

        if (isAppStreamFile(fileName)) {
            appStreamData = readFileData(itr);
            hasAppStream = true;
        }
    }
}

QByteArray AppImageInfoReader::readFileData(PayloadIterator& itr) const {
    QByteArray data;
    auto& istream = itr.read();

    char c;
    while (!istream.eof()) {
        istream.get(c);
        data.push_back(c);
    }

    return data;
}

bool AppImageInfoReader::isMainDesktopFile(const QString& fileName) const {
    return !fileName.contains("/") and fileName.endsWith(".desktop", Qt::CaseInsensitive);
}

bool AppImageInfoReader::isAppStreamFile(const QString& fileName) const {
    return fileName.startsWith("usr/share/metainfo") && fileName.endsWith("appdata.xml");
}

QVariantMap AppImageInfoReader::readAppInfo() {
    loadDataFiles();

    QVariantMap appInfo;
    if (hasDesktopEntry) {
        DesktopEntryReader reader(desktopEntryFile, desktopEntryData);
        appInfo = reader.readAppInfo();
    }

    if (hasAppStream) {
        AppStreamReader reader(appStreamData);
        // replace values
        auto appStreamInfo = reader.readAppInfo();
        for (auto itr = appStreamInfo.begin(); itr != appStreamInfo.end(); ++itr) {
            if (!itr.value().isNull())
                appInfo[itr.key()] = itr.value();
        }
    }

    return appInfo;
}
