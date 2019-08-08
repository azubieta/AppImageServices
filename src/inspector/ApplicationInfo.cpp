// libraries
#include <appimage/appimage++.h>
#include <QVariant>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QVariantList>

// local
#include "ApplicationInfo.h"


ApplicationInfo::ApplicationInfo() {}

ApplicationInfo::ApplicationInfo(const class QJsonObject& jsonObject) {
    id = jsonObject.value("id").toString();
    version = jsonObject.value("version").toString();
    name = jsonObject.value("name").toString();
    summary = jsonObject.value("summary").toString();
    license = jsonObject.value("license").toString();


    locales = jsonArrayToStringList(jsonObject.value("locales").toArray());
    categories = jsonArrayToStringList(jsonObject.value("categories").toArray());
    mimeTypes = jsonArrayToStringList(jsonObject.value("mimeTypes").toArray());
    links = jsonObjectToStringsMap(jsonObject.value("links").toObject());
    flags = jsonArrayToStringList(jsonObject.value("flags").toArray());
}


QStringList ApplicationInfo::jsonArrayToStringList(const QJsonArray& array) const {
    QStringList stringList;
    for (const auto& localeObject: array)
        stringList << localeObject.toString();

    return stringList;
}

QMap<QString, QString> ApplicationInfo::jsonObjectToStringsMap(const class QJsonObject& object) const {
    QMap<QString, QString> map;
    for (const QString& entry: object.keys())
        map[entry] = object.value(entry).toString();

    return map;
}

QJsonObject ApplicationInfo::QJsonObject() {
    class QJsonObject object;

    object["id"] = id;
    object["version"] = version;
    object["name"] = name;
    object["summary"] = summary;
    object["license"] = license;
    object["locales"] = QJsonArray::fromStringList(locales);
    object["categories"] = QJsonArray::fromStringList(categories);
    object["mimeTypes"] = QJsonArray::fromStringList(mimeTypes);
    object["links"] = stringsMapToJsonObject(links);
    object["flags"] = QJsonArray::fromStringList(flags);

    return object;
}

class QJsonObject ApplicationInfo::stringsMapToJsonObject(const QMap<QString, QString>& map) const {
    class QJsonObject object;
    for (QMap<QString, QString>::const_iterator it = map.cbegin(), end = map.cend(); it != end; ++it)
        object.insert(it.key(), it.value());

    return object;
}

const QString& ApplicationInfo::getId() const {
    return id;
}

void ApplicationInfo::setId(const QString& id) {
    ApplicationInfo::id = id;
}

const QString& ApplicationInfo::getVersion() const {
    return version;
}

void ApplicationInfo::setVersion(const QString& version) {
    ApplicationInfo::version = version;
}

const QString& ApplicationInfo::getName() const {
    return name;
}

void ApplicationInfo::setName(const QString& name) {
    ApplicationInfo::name = name;
}

const QString& ApplicationInfo::getSummary() const {
    return summary;
}

void ApplicationInfo::setSummary(const QString& summary) {
    ApplicationInfo::summary = summary;
}

const QStringList& ApplicationInfo::getLocales() const {
    return locales;
}

void ApplicationInfo::setLocales(const QStringList& locales) {
    ApplicationInfo::locales = locales;
}

const QString& ApplicationInfo::getLicense() const {
    return license;
}

void ApplicationInfo::setLicense(const QString& license) {
    ApplicationInfo::license = license;
}

const QStringList& ApplicationInfo::getCategories() const {
    return categories;
}

void ApplicationInfo::setCategories(const QStringList& categories) {
    ApplicationInfo::categories = categories;
}

const QStringList& ApplicationInfo::getMimeTypes() const {
    return mimeTypes;
}

void ApplicationInfo::setMimeTypes(const QStringList& mimeTypes) {
    ApplicationInfo::mimeTypes = mimeTypes;
}

const QMap<QString, QString>& ApplicationInfo::getLinks() const {
    return links;
}

void ApplicationInfo::setLinks(const QMap<QString, QString>& links) {
    ApplicationInfo::links = links;
}

void ApplicationInfo::addLink(const QString& type, const QString& url) {
    links[type] = url;
}

const QStringList& ApplicationInfo::getFlags() const {
    return flags;
}

void ApplicationInfo::setFlags(const QStringList& settings) {
    ApplicationInfo::flags = settings;
}
