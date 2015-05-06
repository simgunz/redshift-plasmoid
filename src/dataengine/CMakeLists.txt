set(redshift_engine_SRCS
    redshiftengine.cpp
    redshiftservice.cpp
    redshiftjob.cpp
    redshiftcontroller.cpp
    redshiftcontainer.cpp
)

kconfig_add_kcfg_files(redshift_engine_SRCS ../redshiftsettings.kcfgc)
add_library(plasma_engine_redshift MODULE ${redshift_engine_SRCS})
target_link_libraries(plasma_engine_redshift KF5::ConfigCore KF5::ConfigGui KF5::Plasma Qt5::DBus)

kcoreaddons_desktop_to_json(plasma_engine_redshift plasma-dataengine-redshift.desktop)

install(TARGETS plasma_engine_redshift DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/dataengine)
install(FILES plasma-dataengine-redshift.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
install(FILES redshift.operations DESTINATION ${PLASMA_DATA_INSTALL_DIR}/services)

