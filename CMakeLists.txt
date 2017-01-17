cmake_minimum_required(VERSION 3.1)
project(redshift-plasmoid)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)
include(FeatureSummary)
include(ECMInstallIcons)

set(REQUIRED_QT_VERSION 5.3.0)
set(KF5_MIN_VERSION 5.0.0)

find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core Qml Gui Widgets DBus)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Config CoreAddons Plasma Service)

add_subdirectory(src)
add_subdirectory(icons)
add_subdirectory(po)
