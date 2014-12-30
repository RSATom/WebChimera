cmake_minimum_required( VERSION 2.8.11 )

project( WebChimera )

add_definitions( -std=c++11 )

set( FB_PROJECTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/deps/firebreath )
