# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
list(APPEND EXTRA_COMPONENT_DIRS components/arduino)
list(APPEND EXTRA_COMPONENT_DIRS components/TFT_eSPI)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXE_LINKER_FLAGS "-flto")
project(DisplayFirmware)