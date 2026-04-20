cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0111 OLD)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

idf_build_set_property(PARTITION_TABLE_FILE "partition_table.csv" APPEND)

project(ON2SolutionUI)
