
option (PKG_CONFIG "Create and install a pkg-config file")

if (PKG_CONFIG)
    set (PC_FILE_OUT "${CMAKE_CURRENT_BINARY_DIR}/libeye.pc") 
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libeye.pc.in
                   ${PC_FILE_OUT}
                   @ONLY
                   )

    INSTALL(FILES ${PC_FILE_OUT}
            DESTINATION lib/pkgconfig/
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
            )
endif()
