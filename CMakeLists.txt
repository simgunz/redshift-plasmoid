project(redshift-plasmoid)

find_package(KDE4 REQUIRED QT4 REQUIRED)
include(KDE4Defaults)
include_directories(${QT_INCLUDE} ${KDE4_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(dataengine)
add_subdirectory(applet)
add_subdirectory(signalEmitter)

install(FILES redshift.kcfg DESTINATION ${KCFG_INSTALL_DIR})