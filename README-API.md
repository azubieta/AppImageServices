
## D-Bus Interface
```
org.appimage.Services1.Launcher
\- launch ( String appImagePath , StringList args )
\- register ( String appImagePath )
\- unregister ( String appImagePath )
\- listRegisteredApplications

org.appimage.Services1.Updater
\- checkUpdates ( String appImagePath )
\- update ( String appImagePath )
\- rollback ( String appImagePath )

org.appimage.Services1.Updater.Task
\- progressValue
\- progressTotal
\- status
\- statusMessage

org.appimage.Services1.Inspector
\- getApplicationInfo ( String appImagePath )
\- getDeveloperInfo ( String appImagePath )
\- verifySignature ( String appImagePath )

org.appimamge.Services1.Self
\- checkUpdates
\- update
\- rollback

```

## Command Line Interface
```
cmd=appimage-services
appimage_path=$HOME/RandomApp.AppImage
appimage_id=org.appmakers.CoolApp

# Launcher
$cmd launch $appimage_path
$cmd launch $appimage_id # ?

$cmd register $appimage_path
$cmd unregister $appimage_path

# Directories monitoring
$cmd add-applications-dir $HOME/Applications
$cmd remove-applications-dir $HOME/Applications

# Updates 
$cmd check-updates $appimage_path
$cmd update $appimage_path
$cmd rollback $appimage_path

# Inspection
$cmd inspect $appimage_path # shows appimage contents and metadata
$cmd verify-signature $appimage_path # verify checksums

# Thumbnailer
$cmd create-thumbnail $appimage_path
$cmd remove-thumbnail $appimage_path

# Self Management
$cmd # starts as a service (deamon)
./appimage-services.AppImage # starts as a service (deamon)

./appimage-services.AppImage self-install --move-to-opt
./appimage-services.AppImage self-uninstall
./appimage-services.AppImage self-update
./appimage-services.AppImage self-rollback
```
