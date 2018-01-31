set(SOURCES
    Keyboard.cpp
    )

if(CMAKE_EXTRA_GENERATOR OR MSVC_IDE)
    message(STATUS "Adding header files to project")
    file(GLOB_RECURSE HEADERS "${INCLUDE_DIR}/*.hpp")
    if(MSVC_IDE)
        source_group("Header Files" FILES ${HEADERS})
    endif()
    list(APPEND SOURCES ${CUKE_HEADERS})
endif()

add_library(libledkeyboard STATIC ${SOURCES})

target_link_libraries(libledkeyboard PRIVATE ${DEP_LIBRARIES})
