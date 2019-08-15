// libraries
#include <XdgUtils/DesktopEntry/DesktopEntryStringsValue.h>

// local
#include "DesktopEntryReader.h"

DesktopEntryReader::DesktopEntryReader(const QString& fileName, const QByteArray& desktopEntryData)
        : fileName(fileName), desktopEntryData(desktopEntryData) {}

QVariantMap DesktopEntryReader::readAppInfo() {
    try {
        XdgUtils::DesktopEntry::DesktopEntry desktopEntry(desktopEntryData.toStdString());

        appInfo["id"] = fileName.split("/").last().chopped(8);
        appInfo["name"] = QString::fromStdString(static_cast<std::string>(desktopEntry["Desktop Entry/Name"]));
        appInfo["summary"] = QString::fromStdString(static_cast<std::string>(desktopEntry["Desktop Entry/Comment"]));

        appInfo["categories"] = readDesktopEntryStringList(desktopEntry.get("Desktop Entry/Categories", {}));
        appInfo["mimeTypes"] = readDesktopEntryStringList(desktopEntry.get("Desktop Entry/MimeType", {}));

        appInfo["flags"] = readFlags(desktopEntry);
    } catch (const XdgUtils::DesktopEntry::DesktopEntryError& error) {
        std::cerr << "Error while parsing the Desktop Entry " << error.what() << std::endl;
    }
    
    return appInfo;
}

QStringList DesktopEntryReader::readFlags(XdgUtils::DesktopEntry::DesktopEntry& desktopEntry) const {
    QStringList flags;

    if (desktopEntry.exists("Desktop Entry/Terminal")) {
        if (desktopEntry["Desktop Entry/Terminal"])
            flags += "TERMINAL_APP";
    }

    if (desktopEntry.exists("Desktop Entry/X-AppImage-Integrate")) {
        if (!desktopEntry["Desktop Entry/X-AppImage-Integrate"])
            flags += "NO_INTEGRATE";
    }
    return flags;
}

QStringList DesktopEntryReader::readDesktopEntryStringList(const std::string& string) {
    XdgUtils::DesktopEntry::DesktopEntryStringsValue stringsValue(string);

    QStringList list;
    for (int i = 0; i < stringsValue.size(); ++i)
        list.push_back(QString::fromStdString(stringsValue[i]));

    return list;
}
