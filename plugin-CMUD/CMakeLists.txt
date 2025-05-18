cmake_minimum_required(VERSION 4.0)

project(lxqt-panel)

set(PLUGIN "CMUD")
set(LXQT_CMAKE_MODULES_DIR "/usr/share/cmake/lxqt2-build-tools/modules")
set(CMAKE_INSTALL_FULL_LIBDIR "/usr/lib")
set(CMAKE_INSTALL_FULL_DATAROOTDIR "/usr/share")
set(REQUIRED_QT_VERSION "6.6.0")
set(KF6_MINIMUM_VERSION "6.0.0")
set(CMAKE_AUTOMOC ON)

find_package(Qt6Widgets ${REQUIRED_QT_VERSION} REQUIRED)
find_package(KF6WindowSystem ${KF6_MINIMUM_VERSION} REQUIRED)

include_directories("/usr/include/lxqt/")
set(HEADERS
    cmudplugin.h
)

set(SOURCES
    cmudplugin.cpp
)

set(UIS
)

include("/usr/share/cmake/lxqt2-build-tools/modules/LXQtTranslateTs.cmake")
include("/usr/share/cmake/lxqt2-build-tools/modules/LXQtTranslateDesktop.cmake")
include("/usr/share/cmake/lxqt2-build-tools/modules/LXQtTranslationLoader.cmake")
include ("../BuildPlugin.cmake")
BUILD_LXQT_PLUGIN(${PLUGIN})
