// libraries
#include <gtest/gtest.h>

// local
extern "C" {
#include "appimagelauncher_interface.h"
}


class RuntimeSharedTests : public ::testing::Test {
protected:
};

TEST_F(RuntimeSharedTests, testShouldIntegrationAssistantBeUsedOn) {
    ASSERT_FALSE(shouldIntegrationAssistantBeUsedOn("/usr/local/bin/appimage-services"));
    ASSERT_FALSE(shouldIntegrationAssistantBeUsedOn("/tmp/.mount_"));
    ASSERT_TRUE(shouldIntegrationAssistantBeUsedOn("/usr/local/bin/random.AppImage"));
    setenv("APPIMAGELAUNCHER_DISABLE", "true", 1);
    ASSERT_FALSE(shouldIntegrationAssistantBeUsedOn("/usr/local/bin/random.AppImage"));
}