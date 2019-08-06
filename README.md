# AppImageServices

Collection of D-Bus services providing a high-level interface over the AppImage manipulation libraries. 
It's meant to be used by file managers, software centers and other tools to provide a consistent user experience.

## Features
- Applications launcher registration following the
 [Desktop Entry Specification](https://specifications.freedesktop.org/desktop-entry-spec/latest/) 
 and the [Desktop Menu Specification](https://specifications.freedesktop.org/menu-spec/menu-spec-latest.html)
- Mime-type associations according to the [Shared Mime Info Specification](https://www.freedesktop.org/wiki/Specifications/shared-mime-info-spec/) 
- Applications updates using [AppImageUpdaterBridge](https://github.com/antony-jr/AppImageUpdaterBridge) as backend
- Application metadata extraction
- Custom runtime for AppImage execution (useful to avoid giving execution permissions to every file) 
- System wide installation (applications installed to `/usr/local/bin` and registered for all users)

**TO BE IMPLEMENTED**
- Applications registry (to be able to track every installed application)
- Applications directories monitoring (to enable transparent registration of applications)

For more details about the API see the `README-API.md` file.

**IMPORTANT NOTE:** _**This service is an enhancement to the AppImage user experience but not a requirement to use AppImages.**_

## Build instructions

**Conan Setup** 

This project is configured to be built using [**conan.io**](https://conan.io) 
to install and configure it run: 
```bash=
pip3 install conan

echo "Setup additional conan repositories"
conan remote add appimage-conan-community https://api.bintray.com/conan/appimage-conan-community/public-conan --insert=0
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan --insert=1
```

**Configure and build AppImageServices**
```bash=
# on the project root dir
mkdir build && cd build
# install dependencies (may take a while if there are no binaries for your system)
conan install .. --build missing

# build the project
conan build ..
``` 
