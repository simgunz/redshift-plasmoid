set(redshiftapplet_SRCS redshiftapplet.cpp)
kde4_add_ui_files(redshiftapplet_SRCS redshift.ui)
kde4_add_ui_files(redshiftapplet_SRCS activities.ui)
kde4_add_kcfg_files(redshiftapplet_SRCS ../redshiftsettings.kcfgc)

kde4_add_plugin(plasma_applet_redshift ${redshiftapplet_SRCS})

target_link_libraries(plasma_applet_redshift ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KCMUTILS_LIBRARY})

install(TARGETS plasma_applet_redshift DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-redshift.desktop DESTINATION ${SERVICES_INSTALL_DIR})
