
add_definitions(${VRJUGGLER_DEFINITIONS})
include_directories(${VRJUGGLER_INCLUDE_DIRS})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VRJUGGLER_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VRJUGGLER_CXX_FLAGS}")

add_executable(vrjtest
    GetSurfaces.cpp
    GetSurfaces.h
    TestApp.cpp
    TestApp.h
    vrjtest.cpp)

target_link_libraries(vrjtest ${VRJUGGLER_LIBRARIES})
