add_subdirectory(dataengine)
#add_subdirectory(applet)
add_subdirectory(redshiftAutostartEnabler)

install(FILES redshift.kcfg DESTINATION ${KCFG_INSTALL_DIR})
