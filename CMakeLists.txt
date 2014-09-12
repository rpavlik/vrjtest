
cmake_minimum_required(VERSION 2.8.5)

# Set package properties
project(vrjtest)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(MSVCMultipleProcessCompile)
include(CreateLaunchers)
set(RUNTIME_LIBRARY_DIRS)

find_package(VRJuggler 3.0 EXACT REQUIRED)
list(APPEND RUNTIME_LIBRARY_DIRS ${VRJUGGLER_RUNTIME_LIBRARY_DIRS})

# Build the application
add_subdirectory(src)


create_default_target_launcher(vrjtest
    FORWARD_ARGS
    RUNTIME_LIBRARY_DIRS
    ${RUNTIME_LIBRARY_DIRS}
    WORKING_DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}"
    ENVIRONMENT
    ${VRJUGGLER_ENVIRONMENT})


include(InstallRequiredSystemLibraries)
