set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")

file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/*.cc"
        "${CMAKE_CURRENT_LIST_DIR}/*.hh"
        "${CMAKE_CURRENT_LIST_DIR}/*.h"
        "${CMAKE_CURRENT_LIST_DIR}/*.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/*.S"
)

idf_component_register(
        SRCS ${SOURCES}
        INCLUDE_DIRS .
        REQUIRES foundation
)

target_compile_options(${COMPONENT_LIB} PRIVATE "-w")
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD_REQUIRED ON)
