#/**********************************************************\
# Auto-generated Mac project definition file for the
# WebChimera Plugin project
#\**********************************************************/

# Mac template platform definition CMake file
# Included from ../CMakeLists.txt

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set( QTDIR ${_qt5Core_install_prefix} )

add_subdirectory( ${DEPS_DIR}/QuickLayer "${DEPS_BINARY_DIR}/QuickLayer" )

# remember that the current source dir is the project root; this file is in Mac/
file( GLOB PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    Mac/[^.]*.cpp
    Mac/[^.]*.mm
    Mac/[^.]*.h
    Mac/[^.]*.cmake
    )

source_group( Mac FILES ${PLATFORM} )

set( VLC_PATH "${DEPS_DIR}/VLC-${VLC_VERSION}.app/Contents/MacOS" )

file( GLOB LIBVLC_LIB ${VLC_PATH}/lib/[^.]*.dylib )
file( GLOB LIBVLC_PLUGINS ${VLC_PATH}/plugins/[^.]*.dylib )
file( GLOB LIBVLC_LUA_EXTENSIONS ${VLC_PATH}/share/lua/extensions/[^.]*.luac )
file( GLOB LIBVLC_LUA_MODULES ${VLC_PATH}/share/lua/modules/[^.]*.luac )
file( GLOB LIBVLC_LUA_PLAYLIST ${VLC_PATH}/share/lua/playlist/[^.]*.luac )

set( LIBVLC
    ${LIBVLC_LIB}
    ${LIBVLC_PLUGINS}
    ${LIBVLC_LUA_EXTENSIONS}
    ${LIBVLC_LUA_MODULES}
    ${LIBVLC_LUA_PLAYLIST}
    )

source_group( Libvlc FILES ${LIBVLC} )

set_source_files_properties(
    ${LIBVLC_LIB}
    PROPERTIES
    MACOSX_PACKAGE_LOCATION MacOS/lib
    )

set_source_files_properties(
    ${LIBVLC_PLUGINS}
    PROPERTIES
    MACOSX_PACKAGE_LOCATION MacOS/plugins
    )

set_source_files_properties(
    ${LIBVLC_LUA_EXTENSIONS}
    PROPERTIES
    MACOSX_PACKAGE_LOCATION MacOS/share/lua/extensions/
    )

set_source_files_properties(
    ${LIBVLC_LUA_MODULES}
    PROPERTIES
    MACOSX_PACKAGE_LOCATION MacOS/share/lua/modules/
    )

set_source_files_properties(
    ${LIBVLC_LUA_PLAYLIST}
    PROPERTIES
    MACOSX_PACKAGE_LOCATION MacOS/share/lua/playlist/
    )

if( QT_STATIC )
    set( QML_QTQUICK2
        ${QTDIR}/qml/QtQuick.2/qmldir
        ${QTDIR}/qml/QtQuick.2/plugins.qmltypes
        )

    set( QML_QTQUICK_LAYOUTS
        ${QTDIR}/qml/QtQuick/Layouts/qmldir
        ${QTDIR}/qml/QtQuick/Layouts/plugins.qmltypes
        )

    file( GLOB QML_QTQUICK_CONTROLS
        ${QTDIR}/qml/QtQuick/Controls/qmldir
        ${QTDIR}/qml/QtQuick/Controls/plugins.qmltypes
        ${QTDIR}/qml/QtQuick/Controls/[^.]*.qml
        )
    file( GLOB QML_QTQUICK_CONTROLS_STYLES
        ${QTDIR}/qml/QtQuick/Controls/Styles/[^.]*
        )
    file( GLOB QML_QTQUICK_CONTROLS_STYLES_BASE
        ${QTDIR}/qml/QtQuick/Controls/Styles/Base/[^.]*
        )
    file( GLOB QML_QTQUICK_CONTROLS_STYLES_BASE_IMAGES
        ${QTDIR}/qml/QtQuick/Controls/Styles/Base/Images/[^.]*
        )
    file( GLOB QML_QTQUICK_CONTROLS_PRIVATE
        ${QTDIR}/qml/QtQuick/Controls/Private/[^.]*
        )

    file( GLOB QML_QTGRAPHICALEFFECTS
        ${QTDIR}/qml/QtGraphicalEffects/qmldir
        ${QTDIR}/qml/QtGraphicalEffects/[^.]*.qml
        )
    file( GLOB QML_QTGRAPHICALEFFECTS_PRIVATE
        ${QTDIR}/qml/QtGraphicalEffects/private/[^.]*.qml
        )

    set( QML_MODULES
        ${QML_QTQUICK2}
        ${QML_QTQUICK_LAYOUTS}
        ${QML_QTQUICK_CONTROLS}
        ${QML_QTQUICK_CONTROLS_STYLES}
        ${QML_QTQUICK_CONTROLS_STYLES_BASE}
        ${QML_QTQUICK_CONTROLS_STYLES_BASE_IMAGES}
        ${QML_QTQUICK_CONTROLS_PRIVATE}
        ${QML_QTGRAPHICALEFFECTS}
        ${QML_QTGRAPHICALEFFECTS_PRIVATE}
        )

    source_group( QmlModules FILES ${QML_MODULES} )

    set_source_files_properties(
        ${QML_QTQUICK2}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION MacOS/qml/QtQuick.2
        )

    set_source_files_properties(
        ${QML_QTQUICK_LAYOUTS}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION MacOS/qml/QtQuick/Layouts
        )

    set_source_files_properties(
        ${QML_QTQUICK_CONTROLS}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION MacOS/qml/QtQuick/Controls
        )

    set_source_files_properties(
        ${QML_QTQUICK_CONTROLS_STYLES}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION MacOS/qml/QtQuick/Controls/Styles
        )
    set_source_files_properties(
        ${QML_QTQUICK_CONTROLS_STYLES_BASE}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION MacOS/qml/QtQuick/Controls/Styles/Base
        )
    set_source_files_properties(
        ${QML_QTQUICK_CONTROLS_STYLES_BASE_IMAGES}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION MacOS/qml/QtQuick/Controls/Styles/Base/Images
        )
    set_source_files_properties(
        ${QML_QTQUICK_CONTROLS_PRIVATE}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION MacOS/qml/QtQuick/Controls/Private
        )

    set_source_files_properties(
        ${QML_QTGRAPHICALEFFECTS}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION MacOS/qml/QtGraphicalEffects
        )
    set_source_files_properties(
        ${QML_QTGRAPHICALEFFECTS_PRIVATE}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION MacOS/qml/QtGraphicalEffects/private
        )
endif( QT_STATIC )

set( SOURCES
    ${SOURCES}
    ${PLATFORM}
    ${LIBVLC}
    ${QML_MODULES}
    )

include_directories(
    ${VLC_PATH}/include
    ${QTDIR}/include
    )

# use this to add preprocessor definitions
add_definitions(
)

set( PLIST "Mac/bundle_template/Info.plist" )
set( STRINGS "Mac/bundle_template/InfoPlist.strings" )
set( LOCALIZED "Mac/bundle_template/Localized.r" )

add_mac_plugin( ${PROJECT_NAME} ${PLIST} ${STRINGS} ${LOCALIZED} SOURCES )

# add library dependencies here; leave ${PLUGIN_INTERNAL_DEPS} there unless you know what you're doing!
target_link_libraries( ${PROJECT_NAME}
    ${PLUGIN_INTERNAL_DEPS}
    ${Qt5Gui_PLUGINS}
    ${Qt5Gui_EGL_LIBRARIES}
    ${Qt5Gui_OPENGL_LIBRARIES}
    ${VLC_PATH}/lib/libvlc.dylib
    QuickLayer
    )

if( QT_STATIC )
    find_library( CARBON_FRAMEWORK Carbon )
    find_library( SECURITY_FRAMEWORK Security )
    find_library( QT5_PLATFORM_SUPPORT Qt5PlatformSupport )
    find_library( IOKIT_FRAMEWORK IOKit )
    find_library( ZLIB z )
    find_library( QTHARFBUZZNG qtharfbuzzng )
    find_library( QT_QUICK2_PLUGIN qtquick2plugin PATHS ${QTDIR}/qml/QtQuick.2 )
    find_library( QT_LAYOUTS_PLUGIN qquicklayoutsplugin PATHS ${QTDIR}/qml/QtQuick/Layouts )
    find_library( QT_CONTROLS_PLUGIN qtquickcontrolsplugin PATHS ${QTDIR}/qml/QtQuick/Controls )

    target_link_libraries( ${PROJECT_NAME}
        ${CARBON_FRAMEWORK}
        ${SECURITY_FRAMEWORK}
        ${IOKIT_FRAMEWORK}
        ${QT5_PLATFORM_SUPPORT}
        ${ZLIB}
        ${QTHARFBUZZNG}
        ${QT_QUICK2_PLUGIN}
        ${QT_LAYOUTS_PLUGIN}
        ${QT_CONTROLS_PLUGIN}
        )
endif( QT_STATIC )

#To create a DMG, include the following file
#include( Mac/installer.cmake )
