project(redshift-plasmoid)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include_directories(${KDE4_INCLUDES})

add_subdirectory(dataengine)
add_subdirectory(applet)
add_subdirectory(redshiftAutostartEnabler)
add_subdirectory(icons)
add_subdirectory(po)

install(FILES redshift.kcfg DESTINATION ${KCFG_INSTALL_DIR})
