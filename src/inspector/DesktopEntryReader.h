#pragma once
// system

//libraries
#include <XdgUtils/DesktopEntry/DesktopEntry.h>

//local
#include "AbstractReader.h"


class DesktopEntryReader : public AbstractReader {
public:
    DesktopEntryReader(const QString& fileName, const QByteArray& desktopEntryData);

    QVariantMap readAppInfo() override;

private:
    QString fileName;
    QByteArray desktopEntryData;
    QVariantMap appInfo;

    static QStringList readDesktopEntryStringList(const std::string& string);

    QStringList readFlags(XdgUtils::DesktopEntry::DesktopEntry& desktopEntry) const;
};



