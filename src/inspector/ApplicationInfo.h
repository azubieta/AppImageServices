#pragma once

#include <QMap>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>

class ApplicationInfo {
public:
    ApplicationInfo();


    explicit ApplicationInfo(const QJsonObject& jsonObject);

    class QJsonObject QJsonObject() ;

    const QString& getId() const;

    void setId(const QString& id);

    const QString& getVersion() const;

    void setVersion(const QString& version);

    const QString& getName() const;

    void setName(const QString& name);

    const QString& getSummary() const;

    void setSummary(const QString& summary);

    const QStringList& getLocales() const;

    void setLocales(const QStringList& locales);

    const QString& getLicense() const;

    void setLicense(const QString& license);

    const QStringList& getCategories() const;

    void setCategories(const QStringList& categories);

    const QStringList& getMimeTypes() const;

    void setMimeTypes(const QStringList& mimeTypes);

    const QMap<QString, QString>& getLinks() const;

    void setLinks(const QMap<QString, QString>& links);

    void addLink(const QString& type, const QString& url);

    const QStringList& getFlags() const;

    void setFlags(const QStringList& settings);

private:
    QStringList jsonArrayToStringList(const QJsonArray& array) const;

    QMap<QString, QString> jsonObjectToStringsMap(const class QJsonObject& object) const;

    class QJsonObject stringsMapToJsonObject(const QMap<QString, QString>& map) const;

    QString id;
    QString version;
    QString name;
    QString summary;
    QStringList locales;
    QString license;
    QStringList categories;
    QStringList mimeTypes;
    QMap<QString, QString> links;
    QStringList flags;
};
