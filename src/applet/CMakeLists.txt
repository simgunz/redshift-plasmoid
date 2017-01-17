plasma_install_package(package org.kde.plasma.redshift)

set(redshiftapplet_SRCS
    plugin/redshift.cpp
    plugin/redshiftplugin.cpp
    #redshiftosdwidget.cpp
)

kconfig_add_kcfg_files(redshiftapplet_SRCS ../redshiftsettings.kcfgc)

add_library(redshiftplugin SHARED ${redshiftapplet_SRCS})

target_link_libraries(redshiftplugin
                      Qt5::Qml
                      KF5::Plasma
                      KF5::ConfigGui
)

install(TARGETS redshiftplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/redshift)
install(FILES plugin/qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/redshift)
