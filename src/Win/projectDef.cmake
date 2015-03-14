#/**********************************************************\
# Auto-generated Windows project definition file for the
# WebChimera Plugin project
#\**********************************************************/

# Windows template platform definition CMake file
# Included from ../CMakeLists.txt

# remember that the current source dir is the project root; this file is in Win/
file( GLOB PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    Win/[^.]*.cpp
    Win/[^.]*.h
    Win/[^.]*.rc
    Win/[^.]*.cmake
    )

# use this to add preprocessor definitions
add_definitions(
    /D "_ATL_STATIC_REGISTRY"
    /D "_WIN32_WINNT=0x0501"
    )

# get PCH related files
file( GLOB PCH RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    StdAfx.*
    )

# make sure that PCH-related files is NOT part of ${SOURCES}
LIST( REMOVE_ITEM SOURCES ${PCH} )

SOURCE_GROUP( "PCH files" FILES ${PCH} )
SOURCE_GROUP( Win FILES ${PLATFORM} )

set( SOURCES
    ${SOURCES}
    ${PLATFORM}
    )

# set PCH
MACRO( ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar )
    GET_FILENAME_COMPONENT( PrecompiledBasename ${PrecompiledHeader} NAME_WE )
    SET( __PrecompiledBinary "$(IntDir)\\$(TargetName).pch" )

    SET_SOURCE_FILES_PROPERTIES( ${PrecompiledSource}
        PROPERTIES COMPILE_FLAGS "/Yc -Zm160" )
    #OBJECT_OUTPUTS "${__PrecompiledBinary}"

    foreach( CURFILE ${${SourcesVar}} )
        GET_FILENAME_COMPONENT( CURFILE_EXT ${CURFILE} EXT )
        GET_FILENAME_COMPONENT( CURFILE_NAME ${CURFILE} NAME )
        if( CURFILE_EXT STREQUAL ".cpp" AND NOT CURFILE_NAME STREQUAL PrecompiledBasename )
            SET_SOURCE_FILES_PROPERTIES( ${CURFILE}
                PROPERTIES COMPILE_FLAGS "/Yu /FI\"${PrecompiledHeader}\" -Zm160" )
            #OBJECT_DEPENDS "${__PrecompiledBinary}"
        endif()
    endforeach()

 # Add precompiled header to SourcesVar
    list( APPEND ${SourcesVar} ${PrecompiledSource} )
    list( APPEND ${SourcesVar} ${PrecompiledHeader} )
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)

# activate PCH
ADD_MSVC_PRECOMPILED_HEADER( "stdafx.h" "stdafx.cpp" SOURCES )

add_windows_plugin( ${PROJECT_NAME} SOURCES )

# This is an example of how to add a build step to sign the plugin DLL before
# the WiX installer builds.  The first filename (certificate.pfx) should be
# the path to your pfx file.  If it requires a passphrase, the passphrase
# should be located inside the second file. If you don't need a passphrase
# then set the second filename to "".  If you don't want signtool to timestamp
# your DLL then make the last parameter "".
#
# Note that this will not attempt to sign if the certificate isn't there --
# that's so that you can have development machines without the cert and it'll
# still work. Your cert should only be on the build machine and shouldn't be in
# source control!
# -- uncomment lines below this to enable signing --
#firebreath_sign_plugin( ${PROJECT_NAME}
#    "${CMAKE_CURRENT_SOURCE_DIR}/sign/certificate.pfx"
#    "${CMAKE_CURRENT_SOURCE_DIR}/sign/passphrase.txt"
#    "http://timestamp.verisign.com/scripts/timestamp.dll" )

include_directories( ${DEPS_DIR}/libvlc-sdk/include )

get_property( LINK_FLAGS TARGET ${PROJECT_NAME} PROPERTY LINK_FLAGS )
set( LINK_FLAGS "${LINK_FLAGS} /INCLUDE:__imp__D3DCompile@44" )
set_target_properties( ${PROJECT_NAME} PROPERTIES LINK_FLAGS ${LINK_FLAGS} )

# add library dependencies here; leave ${PLUGIN_INTERNAL_DEPS} there unless you know what you're doing!
target_link_libraries( ${PROJECT_NAME}
    ${PLUGIN_INTERNAL_DEPS}
    ${DEPS_DIR}/libvlc-sdk/lib/msvc/libvlc.lib
    ${Qt5Gui_EGL_LIBRARIES}
    ${Qt5Gui_OPENGL_LIBRARIES}
    d3dcompiler.lib
    )

set( QT_STATIC 1 )

if( QT_STATIC )
    add_definitions( -DQT_OPENGL_ES_2_ANGLE_STATIC )

    find_package( OpenSSL REQUIRED )

    target_link_libraries( ${PROJECT_NAME}
        ${Qt5Gui_PLUGINS}
        Winmm.lib
        Imm32.lib
        D3d9.lib
        dxguid.lib
        strmiids.lib
        ${OPENSSL_LIBRARIES}
        Crypt32.lib
        )

    target_link_libraries( ${PROJECT_NAME} debug "$ENV{QTDIR}/lib/qtharfbuzzngd.lib" )
    target_link_libraries( ${PROJECT_NAME} debug "$ENV{QTDIR}/lib/translatord.lib" )
    target_link_libraries( ${PROJECT_NAME} debug "$ENV{QTDIR}/lib/preprocessord.lib" )
    target_link_libraries( ${PROJECT_NAME} debug "$ENV{QTDIR}/lib/Qt5PlatformSupportd.lib" )
    target_link_libraries( ${PROJECT_NAME} debug "$ENV{QTDIR}/qml/QtQuick.2/qtquick2plugind.lib" )
    target_link_libraries( ${PROJECT_NAME} debug "$ENV{QTDIR}/qml/QtQuick/Layouts/qquicklayoutsplugind.lib" )
    target_link_libraries( ${PROJECT_NAME} debug "$ENV{QTDIR}/qml/QtQuick/Controls/qtquickcontrolsplugind.lib" )

    target_link_libraries( ${PROJECT_NAME} optimized "$ENV{QTDIR}/lib/qtharfbuzzng.lib" )
    target_link_libraries( ${PROJECT_NAME} optimized "$ENV{QTDIR}/lib/translator.lib" )
    target_link_libraries( ${PROJECT_NAME} optimized "$ENV{QTDIR}/lib/preprocessor.lib" )
    target_link_libraries( ${PROJECT_NAME} optimized "$ENV{QTDIR}/lib/Qt5PlatformSupport.lib" )
    target_link_libraries( ${PROJECT_NAME} optimized "$ENV{QTDIR}/qml/QtQuick.2/qtquick2plugin.lib" )
    target_link_libraries( ${PROJECT_NAME} optimized "$ENV{QTDIR}/qml/QtQuick/Layouts/qquicklayoutsplugin.lib" )
    target_link_libraries( ${PROJECT_NAME} optimized "$ENV{QTDIR}/qml/QtQuick/Controls/qtquickcontrolsplugin.lib" )
endif( QT_STATIC )

set( WIX_HEAT_FLAGS
    -gg                 # Generate GUIDs
    -srd                # Suppress Root Dir
    -cg PluginDLLGroup  # Set the Component group name
    -dr INSTALLDIR      # Set the directory ID to put the files in
    )

set( VLC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Win/WiX/vlc-${VLC_VERSION} )

#generate vlc.wxs
execute_process(
    COMMAND ${WIX_HEAT} dir ${VLC_PATH} -cg VLC -dr INSTALLDIR -var var.VLC -template fragment -gg -srd -sfrag -sreg -out vlc.wxs -t vlc.xslt
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Win/Wix
    )

set( WIX_SOURCES_WITH_VLC
    ${CMAKE_CURRENT_SOURCE_DIR}/Win/WiX/ChimeraInstaller.wxs
    ${CMAKE_CURRENT_SOURCE_DIR}/Win/WiX/vlc.wxs
    ${CMAKE_CURRENT_SOURCE_DIR}/Win/WiX/qt.wxs
    )

set( OPENSSL_INCLUDE_DIR ${OPENSSL_INCLUDE_DIR}/.. )

set( WIX_LINK_FLAGS ${WIX_LINK_FLAGS} -dVLC=${VLC_PATH} -dQTDIR=$ENV{QTDIR} -dOPENSSL_ROOT_DIR=${OPENSSL_INCLUDE_DIR} )
set( FB_WIX_DEST ${FB_BIN_DIR}/${PLUGIN_NAME}/${CMAKE_CFG_INTDIR}/${PLUGIN_NAME}_${FBSTRING_PLUGIN_VERSION}_vlc_${VLC_VERSION}.msi )

add_wix_installer( ${PLUGIN_NAME}
    "${WIX_SOURCES_WITH_VLC}"
    PluginDLLGroup
    ${FB_BIN_DIR}/${PLUGIN_NAME}/${CMAKE_CFG_INTDIR}/
    ${FB_BIN_DIR}/${PLUGIN_NAME}/${CMAKE_CFG_INTDIR}/${FBSTRING_PluginFileName}.dll
    ${PROJECT_NAME}
    )

# This is an example of how to add a build step to sign the WiX installer
# -- uncomment lines below this to enable signing --
#firebreath_sign_file("${PLUGIN_NAME}_WiXInstall"
#    "${FB_BIN_DIR}/${PLUGIN_NAME}/${CMAKE_CFG_INTDIR}/${PLUGIN_NAME}.msi"
#    "${CMAKE_CURRENT_SOURCE_DIR}/sign/certificate.pfx"
#    "${CMAKE_CURRENT_SOURCE_DIR}/sign/passphrase.txt"
#    "http://timestamp.verisign.com/scripts/timestamp.dll")

# This is an example of how to create a cab
# -- uncomment lines below this to enable signing --
#create_cab(${PLUGIN_NAME}
#    ${CMAKE_CURRENT_SOURCE_DIR}/Win/Wix/Chimera.ddf
#    ${CMAKE_CURRENT_SOURCE_DIR}/Win/Wix/Chimera.inf
#    ${FB_BIN_DIR}/${PLUGIN_NAME}/${CMAKE_CFG_INTDIR}/
#    ${PROJECT_NAME}_WiXInstallExe
#    )
