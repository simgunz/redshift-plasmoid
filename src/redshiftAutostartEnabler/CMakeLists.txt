set(redshiftautostart_SRCS
    redshiftautostart.cpp
    redshiftenabler.cpp
)

add_executable(redshiftautostart ${redshiftautostart_SRCS})

target_link_libraries(redshiftautostart Qt5::Core Qt5::DBus)

install(TARGETS redshiftautostart DESTINATION ${BIN_INSTALL_DIR})
install(FILES RedshiftAutostart.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/autostart)
