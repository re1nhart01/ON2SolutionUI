cmake_minimum_required(VERSION 3.16)

file(GLOB_RECURSE FOUNDATION_SOURCES
        "*.cpp" "*.cc" "*.c"
        "*.hpp" "*.hh" "*.h"
)

idf_component_register(
        SRCS ${FOUNDATION_SOURCES}
        INCLUDE_DIRS .
        REQUIRES
        lvgl
        esp_lcd
        driver
        esp_timer
        esp_lcd_touch_gt911
)

target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_20)

# ---- Warnings ----
#if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
#    target_compile_options(foundation PRIVATE -Wall -Wextra -Wpedantic)
#elseif (MSVC)
#    target_compile_options(foundation PRIVATE /W4)
#endif()
