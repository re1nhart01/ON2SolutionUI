set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR})
file(GLOB_RECURSE SOURCES
        "*.c"
        "*.cpp"
        "*.cc"
        "*.S"
)

idf_component_register(
        SRCS ${SOURCES}
        INCLUDE_DIRS .
        REQUIRES foundation
)

littlefs_create_partition_image(storage ../flash_data FLASH_IN_PROJECT)
target_compile_options(${COMPONENT_LIB} PRIVATE
        -ffunction-sections
        -fdata-sections
)
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD_REQUIRED ON)
