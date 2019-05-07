# Configure CMAKE
cmake_policy(SET CMP0048 NEW)
cmake_minimum_required(VERSION 3.10)

# Project
project(AppImageServices VERSION 0.1.0 LANGUAGES CXX DESCRIPTION "Language agnostic implementation of the AppImage user experience")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

add_subdirectory(src)
add_subdirectory(res)
