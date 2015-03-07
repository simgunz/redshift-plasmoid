set(redshiftautostart_SRCS
    redshiftautostart.cpp
    redshiftenabler.cpp
)

add_executable(redshiftautostart ${redshiftautostart_SRCS})

target_link_libraries(redshiftautostart  )

install(TARGETS redshiftautostart DESTINATION ${BIN_INSTALL_DIR})
install(FILES RedshiftAutostart.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/autostart)
