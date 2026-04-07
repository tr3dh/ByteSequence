cmake_minimum_required(VERSION 3.16)
project(ByteSequence)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/AggressiveInterfaceOptimizations.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/provideLibrarys.cmake)

set(CMAKE_CXX_STANDARD 23)

set(INCLUDES "")
set(CPPOBJS "")
set(LINKLIBS "")

provideHeaderOnlyLib("${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/pfr" "https://github.com/apolukhin/magic_get" "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/pfr/include" "" "")

add_library(ByteSequence INTERFACE)

# Includes (für Nutzer sichtbar)
target_include_directories(ByteSequence INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:src>
)

target_include_directories(ByteSequence INTERFACE ${INCLUDES})

# apply_aggressive_optimizations_to_interface(ByteSequence INTERFACE)