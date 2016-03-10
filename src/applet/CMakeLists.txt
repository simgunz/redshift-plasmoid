#set(redshiftapplet_SRCS
#    redshiftapplet.cpp
    #redshiftosdwidget.cpp
#)

#qt5_wrap_ui(redshiftapplet_SRCS redshift.ui)
#qt5_wrap_ui(redshiftapplet_SRCS activities.ui)
#kconfig_add_kcfg_files(redshiftapplet_SRCS ../redshiftsettings.kcfgc)
#add_library(plasma_applet_redshift MODULE ${redshiftapplet_SRCS})

#target_link_libraries(plasma_applet_redshift KF5::Plasma)

#install(TARGETS plasma_applet_redshift DESTINATION ${PLUGIN_INSTALL_DIR})
#install(FILES plasma-applet-redshift.desktop DESTINATION ${SERVICES_INSTALL_DIR})

plasma_install_package(package org.kde.plasma.redshift)
