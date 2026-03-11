cmake_minimum_required(VERSION 3.24.0)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(IDF_TARGET esp32)

set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_LIST_DIR}/Filter)

project(inmp441_test)