#pragma once
// system
#include <string>

// libraries
#include <appimage/desktop_integration/IntegrationManager.h>

class LauncherService {
public:
    bool registerApp(const std::string& appImagePath) const;

    bool unregisterApp(const std::string& appImagePath) const;

    bool isRegistered(const std::string& appImagePath) const;

    bool launch(const std::string& path, const QStringList& args) const;

private:
    appimage::desktop_integration::IntegrationManager integrationManager;

private:
    void registerAppAddRemoveDesktopEntryAction(const std::string& appImagePath) const;
};

