#/**********************************************************\ 
#
# Auto-Generated Plugin Configuration file
# for Chimera Web Plugin
#
#\**********************************************************/

set(PLUGIN_VERSION "0.1.10")
set(VLC_VERSION "2.1.5")
set(COPYRIGHT_YEARS "2014")

set(PLUGIN_NAME "WebChimera")
set(PLUGIN_PREFIX "WebChimera")
set(PLUGIN_DESCRIPTION "Web Plugin powered by Firebreath/Qt Qml/Vlc")
set(PRODUCT_NAME "Chimera Web Plugin")
set(PROJECT_OWNER "Sergey Radionov")
set(PROJECT_DOMAIN "WebChimera.org")

set(PLUGIN_MIMETYPE "application/x-chimera-plugin")

# ActiveX constants:
set(FBTYPELIB_NAME ${PLUGIN_NAME}Lib)
set(FBTYPELIB_DESC "${PLUGIN_NAME} ${PLUGIN_VERSION} Type Library")
set(IFBControl_DESC "${PLUGIN_NAME} Control Interface")
set(FBControl_DESC "${PLUGIN_NAME} Control Class")
set(IFBComJavascriptObject_DESC "${PLUGIN_NAME} IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "${PLUGIN_NAME} ComJavascriptObject Class")
set(IFBComEventSource_DESC "${PLUGIN_NAME} IFBComEventSource Interface")
set(AXVERSION_NUM "${PLUGIN_VERSION}")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID 2b642482-539b-5158-b60c-725219b3a4b6)
set(IFBControl_GUID 0d25ef13-5e83-527e-822e-e97f89d9055f)
set(FBControl_GUID df4501d0-6a9f-5d59-a08d-7be21a13c847)
set(IFBComJavascriptObject_GUID 3321ffa1-0fcd-5c02-bf77-a966735a2296)
set(FBComJavascriptObject_GUID d873591c-3064-53b2-afe9-e67d54974423)
set(IFBComEventSource_GUID 90a5b047-5212-5080-b565-1a0d7bdc5946)
if( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 42cda03d-d9b1-57ce-a9d0-63417b421eb2)
else( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 48998e50-3e9b-5e09-b690-ef815564dfb0)
endif( FB_PLATFORM_ARCH_32 )

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID "${PLUGIN_NAME}.${PLUGIN_NAME}")
if( FB_PLATFORM_ARCH_32 )
    set(MOZILLA_PLUGINID "${PROJECT_DOMAIN}/${PLUGIN_NAME}")  # No 32bit postfix to maintain backward compatability.
else( FB_PLATFORM_ARCH_32 )
    set(MOZILLA_PLUGINID "${PROJECT_DOMAIN}/${PLUGIN_NAME}_${FB_PLATFORM_ARCH_NAME}")
endif( FB_PLATFORM_ARCH_32 )

# strings
set(FBSTRING_CompanyName "${PROJECT_OWNER}")
set(FBSTRING_PluginDescription "${PLUGIN_DESCRIPTION}")
set(FBSTRING_PLUGIN_VERSION "${PLUGIN_VERSION}")
set(FBSTRING_LegalCopyright "Copyright ${COPYRIGHT_YEARS} ${PROJECT_OWNER}")
if( WIN32 )
    set(FBSTRING_PluginFileName "np${PLUGIN_NAME}.dll")
elseif( APPLE )
    set(FBSTRING_PluginFileName "${PLUGIN_NAME}")
else()
    set(FBSTRING_PluginFileName "np${PLUGIN_NAME}")
endif()
set(FBSTRING_ProductName "${PRODUCT_NAME}")
set(FBSTRING_FileExtents "")
if( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "${PLUGIN_NAME}")  # No 32bit postfix to maintain backward compatability.
else( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "${PLUGIN_NAME} ${FB_PLATFORM_ARCH_NAME}")
endif( FB_PLATFORM_ARCH_32 )
set(FBSTRING_MIMEType "${PLUGIN_MIMETYPE}")

# Uncomment this next line if you're not planning on your plugin doing
# any drawing:

#set (FB_GUI_DISABLED 1)

# Mac plugin settings. If your plugin does not draw, set these all to 0
set(FBMAC_USE_QUICKDRAW 0)
set(FBMAC_USE_CARBON 0)
set(FBMAC_USE_COCOA 1)
set(FBMAC_USE_COREGRAPHICS 1)
set(FBMAC_USE_COREANIMATION 0)
set(FBMAC_USE_INVALIDATINGCOREANIMATION 0)

# If you want to register per-machine on Windows, uncomment this line
#set (FB_ATLREG_MACHINEWIDE 1)
