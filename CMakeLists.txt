cmake_minimum_required(VERSION 3.1)
project(dmb VERSION 0.0.2 LANGUAGES C)

set(dmb_version_major "0")
set(dmb_version_minor "1")
set(dmb_version_micro "1")
set(dmb_version "${dmb_version_major}.${dmb_version_minor}.${dmb_version_micro}")

# Set default output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

#Add compiler flags
set(CMAKE_C_STANDARD 11)
add_compile_options(-Werror -fPIC)

include(GNUInstallDirs)
add_subdirectory(panel-plugin)

# uninstall target
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/uninstall.cmake
        IMMEDIATE @ONLY)
add_custom_target(uninstall
        ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake/uninstall.cmake)
