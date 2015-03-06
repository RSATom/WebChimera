#/**********************************************************\
# Auto-generated X11 project definition file for the
# WebChimera Plugin project
#\**********************************************************/

# X11 template platform definition CMake file
# Included from ../CMakeLists.txt

# remember that the current source dir is the project root; this file is in X11/
file (GLOB PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    X11/[^.]*.cpp
    X11/[^.]*.h
    X11/[^.]*.cmake
    )

SOURCE_GROUP(X11 FILES ${PLATFORM})

# use this to add preprocessor definitions
add_definitions( -std=c++11 -Werror=return-type )

set (SOURCES
    ${SOURCES}
    ${PLATFORM}
    )

include_directories( ${GTK_INCLUDE_DIRS} )

add_x11_plugin(${PROJECT_NAME} SOURCES)

# add library dependencies here; leave ${PLUGIN_INTERNAL_DEPS} there unless you know what you're doing!
target_link_libraries(${PROJECT_NAME}
    ${PLUGIN_INTERNAL_DEPS}
    vlc
    )

if( Qt5Core_VERSION VERSION_LESS "5.3.0" )
    target_link_libraries( ${PROJECT_NAME} GL )
endif()
