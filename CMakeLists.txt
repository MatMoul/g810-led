cmake_minimum_required(VERSION 3.1)

project(g810-led)

option(USE_LIBUSB  "Use libusb instead of hidapi" OFF)

#
# Generic Compiler Flags
#

set(CMAKE_CXX_STANDARD 11) #17?
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wextra -pedantic ${CMAKE_CXX_FLAGS_DEBUG}")

if(USE_LIBUSB)
    add_definitions(-Dlibusb)
    set(DEP_LIBRARIES "usb-1.0")
else()
    add_definitions(-Dhidapi)
    set(DEP_LIBRARIES "hidapi-hidraw")
endif()

#
# version
#

set(MAJOR 0)
set(MINOR 2)
set(MICRO 7)
add_definitions(-DVERSION="${MAJOR}.${MINOR}.${MICRO}")

#
# Colored Terminal Output
#

if(UNIX AND (
       (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9))
    AND CMAKE_GENERATOR STREQUAL "Ninja")
  # These compilers generate coloured output, but by default only when their output channel is a
  # terminal (TTY/PTY). Ninja however captures all output in a pipe (per-subprocess), disabling
  # coloured compiler diagnostics. We forcefully enable it because Ninja, since 1.0.0
  # (ninja-build/ninja#198) takes care to strip VT100 CSI control sequences from the output if Ninja
  # itself is writing to a pipe instead of a terminal. As a result this should give us the best of
  # both worlds: coloured output when we're running in a terminal, plain output for editors, IDEs,
  # etc.
  set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always ${CMAKE_CXX_FLAGS}" CACHE STRING "" FORCE)
endif()

#
# lib 
#

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${INCLUDE_DIR})
add_subdirectory(src)

#
# app
#

add_subdirectory(app)

