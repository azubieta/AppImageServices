// libraries
#include <gtest/gtest.h>
#include <QtCore/QFile>
#include <QDebug>

// local
#include "AppStreamReader.h"

class AppStreamReaderTests : public ::testing::Test {
public:
    AppStreamReaderTests() {
        QFile f(APPSTREAM_FILE);
        if (f.open(QIODevice::ReadOnly)) {
            auto appStreamData = f.readAll();
            AppStreamReader reader(appStreamData);

            appInfo = reader.readAppInfo();
        }
    }

protected:
    QVariantMap appInfo;
};

TEST_F(AppStreamReaderTests, readId) {
    ASSERT_EQ(appInfo["id"].toByteArray(), "org.kde.kdenlive");
}

TEST_F(AppStreamReaderTests, readName) {
    ASSERT_EQ(appInfo["name"].toByteArray(), QString("Kdenlive"));
}

TEST_F(AppStreamReaderTests, readLicense) {
    ASSERT_EQ(appInfo["license"].toByteArray(), QString("GPL-2.0+"));
}

TEST_F(AppStreamReaderTests, readSummary) {
    ASSERT_EQ(appInfo["summary"].toByteArray(), QString("Video Editor"));
}

TEST_F(AppStreamReaderTests, readLinks) {
    QVariantMap expected = {
            {"bugtracker", "https://bugs.kde.org"},
            {"donation",   "https://www.kde.org/community/donations/?app=kdenlive&source=appdata"},
            {"help",       "https://userbase.kde.org/Kdenlive/Manual"},
            {"homepage",   "https://kdenlive.org/"},
    };
    ASSERT_EQ(appInfo["links"].toMap(), expected);
}

TEST_F(AppStreamReaderTests, readVersion) {
    ASSERT_EQ(appInfo["version"].toByteArray(), QString("19.04.3"));
}