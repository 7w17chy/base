cmake_minimum_required(VERSION 3.1
)
project(errors VERSION 1.0.1 DESCRIPTION "Error handling, the SerenityOS way.")

include(GNUInstallDirs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Werror -Wextra")

add_library(errors STATIC src/errors.cpp)

set_target_properties(errors PROPERTIES
                      VERSION ${PROJECT_VERSION}
                      SOVERSION 1
                      PUBLIC_HEADER errors.hpp)

target_include_directories(errors PRIVATE inc)

install(TARGETS errors
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
