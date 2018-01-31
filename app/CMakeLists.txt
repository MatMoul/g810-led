set(SOURCES
    main.cpp
    utils.cpp
    help.cpp    
    )

if(CMAKE_EXTRA_GENERATOR OR MSVC_IDE)
    message(STATUS "Adding header files to project")
    file(GLOB_RECURSE HEADERS "${INCLUDE_DIR}/*.h")
    if(MSVC_IDE)
        source_group("Header Files" FILES ${HEADERS})
    endif()
    list(APPEND SOURCES ${HEADERS})
endif()

add_executable(ledkeyboard ${SOURCES})
target_link_libraries(ledkeyboard PUBLIC libledkeyboard)

