#/**********************************************************\ 
# 
# Auto-generated CMakeLists.txt for the WebChimera Plugin project
#
#\**********************************************************/

cmake_minimum_required( VERSION 2.8.11 )

Project( ${PLUGIN_NAME} )

set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )

set( DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps )
set( DEPS_BINARY_DIR ${CMAKE_BINARY_DIR}/deps )

find_package( Qt5Core )
find_package( Qt5Qml )
find_package( Qt5Quick )

file( GLOB GENERAL RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    [^.]*.cpp
    [^.]*.h
    [^.]*.cmake
    [^.]*.htm
    [^.]*.txt
    )

file( GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    [^.]*.qrc
    )
qt5_add_resources( GENERATED ${QT_RESOURCES} )
file( GLOB QT_QML_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    [^.]*.qml
    )
set( QT_RESOURCES ${QT_RESOURCES} ${QT_QML_RESOURCES} )
source_group( QtResources FILES ${QT_RESOURCES} )

file( GLOB QML_VLC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../deps/QmlVlc/[^.]*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../deps/QmlVlc/[^.]*.h
    )
file( GLOB QML_VLC_MM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../deps/QmlVlc/QmlVlcMm*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../deps/QmlVlc/QmlVlcMm*.h
    )
list( REMOVE_ITEM QML_VLC ${QML_VLC_MM} )
source_group( QmlVlc FILES ${QML_VLC} )

file( GLOB LIBVLC_WRAPPER RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../deps/QmlVlc/libvlc_wrapper/[^.]*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../deps/QmlVlc/libvlc_wrapper/[^.]*.h
    )
source_group( libvlc_wrapper FILES ${LIBVLC_WRAPPER} )

include_directories(
    ${PLUGIN_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../deps
    ${CMAKE_CURRENT_SOURCE_DIR}/../deps/QmlVlc )

# Generated files are stored in ${GENERATED} by the project configuration
SET_SOURCE_FILES_PROPERTIES(
    ${GENERATED}
    PROPERTIES
    GENERATED 1
    )

SOURCE_GROUP( Generated FILES
    ${GENERATED}
    )

SET( SOURCES
    ${GENERAL}
    ${LIBVLC_WRAPPER}
    ${QT_RESOURCES}
    ${QML_VLC}
    ${GENERATED}
    ${CMAKE_CURRENT_SOURCE_DIR}/../history.txt
    )

# This will include Win/projectDef.cmake, X11/projectDef.cmake, Mac/projectDef 
# depending on the platform
include_platform()

qt5_use_modules( ${PLUGIN_NAME} Qml Quick Gui )

#this will build a Chrome package
#add_chrome_package(${PLUGIN_NAME}
#	${FB_BIN_DIR}/${PLUGIN_NAME}/${CMAKE_CFG_INTDIR}/
#	"${FB_BIN_DIR}/${PLUGIN_NAME}/${CMAKE_CFG_INTDIR}/${FBSTRING_PluginFileName}.dll"
#	"${CMAKE_CURRENT_SOURCE_DIR}/sign/package_key.pem"
#	${PROJECT_NAME})

#this will build a XPI package using XPISigner (see http://code.google.com/p/xpisigner/ )
# add_signed_xpi_installer(${PLUGIN_NAME}
# 	${FB_BIN_DIR}/${PLUGIN_NAME}/${CMAKE_CFG_INTDIR}/
#   "${FB_BIN_DIR}/${PLUGIN_NAME}/${CMAKE_CFG_INTDIR}/${FBSTRING_PluginFileName}.dll"
# 	$ENV{XPI_PATH}
# 	"${CMAKE_CURRENT_SOURCE_DIR}/sign/certificate.pfx"
# 	"${CMAKE_CURRENT_SOURCE_DIR}/sign/passphrase.txt"
# 	${PROJECT_NAME})

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
