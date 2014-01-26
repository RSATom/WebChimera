#/**********************************************************\ 
# 
# Auto-generated CMakeLists.txt for the Chimera Web Plugin project
#
#\**********************************************************/

# Written to work with cmake 2.6
cmake_minimum_required (VERSION 2.6)
set(CMAKE_BACKWARDS_COMPATIBILITY 2.6)

Project(${PLUGIN_NAME})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Multimedia)
find_package(Qt5Qml)
find_package(Qt5Quick)

file(GLOB GENERAL RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    [^.]*.cpp
    [^.]*.h
    [^.]*.cmake
    [^.]*.htm
    [^.]*.txt
    )

file(GLOB LIBVLC_WRAPPER RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    libvlc_wrapper/[^.]*.cpp
    libvlc_wrapper/[^.]*.h
    )
source_group(libvlc_wrapper FILES ${LIBVLC_WRAPPER})

file(GLOB QML_VLC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    QmlVlc/[^.]*.cpp
    QmlVlc/[^.]*.h
    QmlVlc/[^.]*.qml
    )
file(GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    QmlVlc/[^.]*.qrc
    )
qt5_add_resources(QML_VLC ${QT_RESOURCES})
source_group(QmlVlc FILES ${QML_VLC} ${QT_RESOURCES})

include_directories(${PLUGIN_INCLUDE_DIRS})

# Generated files are stored in ${GENERATED} by the project configuration
SET_SOURCE_FILES_PROPERTIES(
    ${GENERATED}
    PROPERTIES
    GENERATED 1
    )

SOURCE_GROUP(Generated FILES
    ${GENERATED}
    )

SET(SOURCES
    ${GENERAL}
    ${LIBVLC_WRAPPER}
    ${QML_VLC}
    ${GENERATED}
    )

# This will include Win/projectDef.cmake, X11/projectDef.cmake, Mac/projectDef 
# depending on the platform
include_platform()

qt5_use_modules(${PLUGIN_NAME} Multimedia Qml Quick Gui)

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
