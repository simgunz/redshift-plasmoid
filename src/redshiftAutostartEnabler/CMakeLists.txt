set(redshiftautostart_SRCS
    redshiftautostart.cpp
    redshiftenabler.cpp
)

add_library(redshiftautostart MODULE ${redshiftautostart_SRCS})
#target_link_libraries(redshiftautostart KF5::Core )

install(TARGETS redshiftautostart DESTINATION ${BIN_INSTALL_DIR})
install(FILES RedshiftAutostart.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/autostart)
