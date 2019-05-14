FROM conanio/gcc48

RUN pip3 install --upgrade pip
RUN pip3 install conan --upgrade
RUN pip3 install conan_package_tools

RUN conan user

RUN conan remote add appimage-conan-community https://api.bintray.com/conan/appimage-conan-community/public-conan --insert=0
RUN conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan --insert=1
# Original cmake binaries are faulty forcing re-build
RUN conan install cmake_installer/3.14.3@conan/stable --build cmake_installer

RUN conan install qt/5.12.3@appimage-conan-community/stable
RUN conan install xdg-utils-cxx/0.1.1@appimage-conan-community/stable
RUN conan install libappimage/1.0.1@appimage-conan-community/stable
RUN conan install squashfuse/0.1.103@appimage-conan-community/stable

