// libraries
#include <gtest/gtest.h>
#include <QTemporaryDir>
#include <appimage/appimage.h>
// local
#include <Installer.h>

class AppImageServicesTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        if (temporaryDir.isValid()) {
            QDir temporaryDirHandle(temporaryDir.path());
            temporaryDirHandle.mkdir("share");
        }
    }

    QTemporaryDir temporaryDir;
};

TEST_F(AppImageServicesTests, install) {
    std::string prefix = temporaryDir.path().toStdString();
    Installer installer(temporaryDir.path());
    installer.install(TESTS_DATA_TYPE2_APPIMAGE);

    QString targetBinPath = temporaryDir.path() + "/bin/type2_x86_64.AppImage";
    ASSERT_TRUE(QFile::exists(targetBinPath));

    char* pathMd5 = appimage_get_md5(targetBinPath.toLocal8Bit());
    QString desktopEntryPath =
            temporaryDir.path() + "/share/applications/appimagekit_" + pathMd5 + "-appimagetool.desktop";
    ASSERT_TRUE(QFile::exists(desktopEntryPath));

    QString inconEntryPath =
            temporaryDir.path() + "/share/icons/hicolor/scalable/apps/appimagekit_" + pathMd5 + "_appimagetool.svg";
    ASSERT_TRUE(QFile::exists(inconEntryPath));
}