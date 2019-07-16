// system
#include <iostream>
#include <fstream>

// libraries
#include <QDebug>
#include <QProcess>
#include <QCoreApplication>
#include <appimage/appimage.h>
#include <XdgUtils/DesktopEntry/DesktopEntry.h>
#include <XdgUtils/DesktopEntry/DesktopEntryStringsValue.h>
#include <QSettings>

// local
#include "LauncherService.h"
#include "LauncherDefines.h"
#include "../updater/UpdaterDefines.h"

bool LauncherService::registerApp(const std::string& appImagePath) const {
    try {
        appimage::core::AppImage appImage(appImagePath);
        integrationManager.registerAppImage(appImage);
        registerAppAddRemoveDesktopEntryAction(appImagePath);
        registerAppAddUpdateDesktopEntryAction(appImagePath);
        return true;
    } catch (const appimage::core::AppImageError& ex) {
        std::cerr << "Unable to register: " << appImagePath << " : " << ex.what() << std::endl;
    }
    return false;
}

bool LauncherService::unregisterApp(const std::string& appImagePath) const {
    try {
        integrationManager.unregisterAppImage(appImagePath);
        return true;
    } catch (const appimage::core::AppImageError& ex) {
        std::cerr << "Unable to register: " << appImagePath << " : " << ex.what() << std::endl;
    }
    return false;
}

bool LauncherService::isRegistered(const std::string& appImagePath) const {
    try {
        return integrationManager.isARegisteredAppImage(appImagePath);
    } catch (const appimage::core::AppImageError& ex) {
        std::cerr << "Unable to check if " << appImagePath << " is registered in the system: " << ex.what()
                  << std::endl;
    }
    return false;
}

void LauncherService::registerAppAddRemoveDesktopEntryAction(const std::string& appImagePath) const {
    using namespace XdgUtils::DesktopEntry;

    char* desktopFilePath = appimage_registered_desktop_file_path(appImagePath.c_str(), nullptr, false);
    if (desktopFilePath != nullptr) {
        QSettings settings;
        std::ifstream ifstream(desktopFilePath);
        DesktopEntry entry(ifstream);

        std::string actionsString = static_cast<std::string>(entry["Desktop Entry/Actions"]);
        DesktopEntryStringsValue actions(actionsString);

        // Add uninstall action
        actions.append("Remove");
        entry.set("Desktop Entry/Actions", actions.dump());

        entry.set("Desktop Action Remove/Name", "Remove");
        entry.set("Desktop Action Remove/Icon", "application-x-trash");

        // Build Remove Action Exec Value
        QString removeCommand = settings.value("Launcher/RemoveCommand", "").toString();
        QString removeCommandArgs = settings.value("Launcher/RemoveCommandArgs", "").toString();

        // Use direct dbus calls as fallback method
        if (removeCommand.isEmpty()) {
            removeCommand = "dbus-send";
            removeCommandArgs = QString("--type=method_call --dest=%1 %2 %3 string:'%4'")
                .arg(LAUNCHER_DBUS_INTERFACE_NAME)
                .arg(LAUNCHER_DBUS_OBJECT_PATH)
                .arg(LAUNCHER_DBUS_METHOD_UNREGISTER_APP)
                .arg(QString::fromStdString(appImagePath));
        } else {
            // replace appimage path argument
            removeCommandArgs = removeCommandArgs.arg(QString::fromStdString(appImagePath));
        }


        entry.set("Desktop Action Remove/Exec", (removeCommand + " " + removeCommandArgs).toStdString());
        entry.set("Desktop Action Remove/TryExec", removeCommand.toStdString());

        std::ofstream ofstream(desktopFilePath);
        ofstream << entry;
    }
}

void LauncherService::registerAppAddUpdateDesktopEntryAction(const std::string& appImagePath) const {
    using namespace XdgUtils::DesktopEntry;

    char* desktopFilePath = appimage_registered_desktop_file_path(appImagePath.c_str(), nullptr, false);
    if (desktopFilePath != nullptr) {
        QSettings settings;
        std::ifstream ifstream(desktopFilePath);
        DesktopEntry entry(ifstream);

        std::string actionsString = static_cast<std::string>(entry["Desktop Entry/Actions"]);
        DesktopEntryStringsValue actions(actionsString);

        // Add Update action
        actions.append("Update");
        entry.set("Desktop Entry/Actions", actions.dump());

        entry.set("Desktop Action Update/Name", "Update");
        entry.set("Desktop Action Update/Icon", "update-none");

        // Read custom commands
        QString updateCommand = settings.value("Launcher/UpdateCommand", "").toString();
        QString updateCommandArgs = settings.value("Launcher/UpdateCommandArgs", "").toString();

        // Use direct dbus calls as fallback method
        if (updateCommand.isEmpty()) {
            updateCommand = "dbus-send";
            updateCommandArgs = QString("--type=method_call --dest=%1 %2 %3 string:'%4'")
                .arg(UPDATER_DBUS_INTERFACE_NAME)
                .arg(UPDATER_TASK_DBUS_OBJECT_PATH)
                .arg(UPDATER_DBUS_METHOD_UPDATE)
                .arg(QString::fromStdString(appImagePath));
        } else {
            // replace appimage path argument
            updateCommandArgs = updateCommandArgs.arg(QString::fromStdString(appImagePath));
        }


        entry.set("Desktop Action Update/Exec", (updateCommand + " " + updateCommandArgs).toStdString());
        entry.set("Desktop Action Update/TryExec", updateCommand.toStdString());

        std::ofstream ofstream(desktopFilePath);
        ofstream << entry;
    }
}

bool LauncherService::launch(const std::string& path, const QStringList& args) const {
    try {
        appimage::core::AppImage appImage(path);

        // Set TARGET_APPIMAGE environment variable
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("TARGET_APPIMAGE", QString::fromStdString(path));

        // Configure runtime process
        QProcess process;
        process.setProcessEnvironment(env);
        process.setArguments(args);

        switch (appImage.getFormat()) {
            case appimage::core::AppImageFormat::TYPE_1:
                process.setProgram("appimage-type1-runtime");
                break;
            case appimage::core::AppImageFormat::TYPE_2:
                process.setProgram("appimage-type2-runtime");
                break;
            default:
                qWarning() << "Unsupported AppImage format";
                return false;
        }

        return process.startDetached();
    } catch (appimage::core::AppImageError& error) {
        return false;
    }


}
