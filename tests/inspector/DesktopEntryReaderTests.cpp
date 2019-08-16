// libraries
#include <gtest/gtest.h>
#include <QtCore/QFile>
#include <QDebug>

// local
#include "DesktopEntryReader.h"

class DesktopEntryReaderTests : public ::testing::Test {
public:
    DesktopEntryReaderTests() {
        QFile f(DESKTOPENTRY_FILE);
        if (f.open(QIODevice::ReadOnly)) {
            auto DesktopEntryData = f.readAll();
            DesktopEntryReader reader(DESKTOPENTRY_FILE, DesktopEntryData);

            appInfo = reader.readAppInfo();
        }
    }

protected:
    QVariantMap appInfo;
};

TEST_F(DesktopEntryReaderTests, readId) {
    ASSERT_EQ(appInfo["id"].toByteArray(), "entry");
}

TEST_F(DesktopEntryReaderTests, readName) {
    ASSERT_EQ(appInfo["name"].toByteArray(), "Subsurface");
}

TEST_F(DesktopEntryReaderTests, readSummary) {
    ASSERT_EQ(appInfo["summary"].toByteArray(), "manage and display dive computer data");
}

TEST_F(DesktopEntryReaderTests, readCategories) {
    ASSERT_EQ(appInfo["categories"].toList(), QList<QVariant>({"Utility"}));
}

TEST_F(DesktopEntryReaderTests, readMimeTypes) {
    ASSERT_EQ(appInfo["mimeTypes"].toList(), QList<QVariant>({"x-fake-mime"}));
}

TEST_F(DesktopEntryReaderTests, readFlags) {
    ASSERT_EQ(appInfo["flags"].toList(), QList<QVariant>({"TERMINAL_APP", "NO_INTEGRATE"}));
}