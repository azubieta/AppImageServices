#pragma once
// system
#include <string>

// libraries
#include <appimage/desktop_integration/IntegrationManager.h>

class LauncherService {
public:
    bool registerApp(const std::string& appImagePath) const;

    bool unregisterApp(const std::string& appImagePath) const;

private:
    appimage::desktop_integration::IntegrationManager integrationManager;

private:
    void registerAppAddRemoveDesktopEntryAction(const std::string& appImagePath) const;
};

