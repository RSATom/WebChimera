#/**********************************************************\ 
#
# Auto-Generated Plugin Configuration file
# for Chimera Web Plugin
#
#\**********************************************************/

set(PLUGIN_NAME "Chimera")
set(PLUGIN_PREFIX "CH_")
set(COMPANY_NAME "RSATom")

# ActiveX constants:
set(FBTYPELIB_NAME ChimeraLib)
set(FBTYPELIB_DESC "Chimera 1.0 Type Library")
set(IFBControl_DESC "Chimera Control Interface")
set(FBControl_DESC "Chimera Control Class")
set(IFBComJavascriptObject_DESC "Chimera IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "Chimera ComJavascriptObject Class")
set(IFBComEventSource_DESC "Chimera IFBComEventSource Interface")
set(AXVERSION_NUM "1")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID 2b642482-539b-5158-b60c-725219b3a4b6)
set(IFBControl_GUID 0d25ef13-5e83-527e-822e-e97f89d9055f)
set(FBControl_GUID df4501d0-6a9f-5d59-a08d-7be21a13c847)
set(IFBComJavascriptObject_GUID 3321ffa1-0fcd-5c02-bf77-a966735a2296)
set(FBComJavascriptObject_GUID d873591c-3064-53b2-afe9-e67d54974423)
set(IFBComEventSource_GUID 90a5b047-5212-5080-b565-1a0d7bdc5946)
if ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 42cda03d-d9b1-57ce-a9d0-63417b421eb2)
else ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 48998e50-3e9b-5e09-b690-ef815564dfb0)
endif ( FB_PLATFORM_ARCH_32 )

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID "RSATom.Chimera")
if ( FB_PLATFORM_ARCH_32 )
    set(MOZILLA_PLUGINID "RSATom.name/Chimera")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(MOZILLA_PLUGINID "RSATom.name/Chimera_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )

# strings
set(FBSTRING_CompanyName "RSATom")
set(FBSTRING_PluginDescription "Web Plugin powered by Firebreath/Qt Qml/Vlc")
set(FBSTRING_PLUGIN_VERSION "1.0.0.0")
set(FBSTRING_LegalCopyright "Copyright 2014 RSATom")
set(FBSTRING_PluginFileName "np${PLUGIN_NAME}")
set(FBSTRING_ProductName "Chimera Web Plugin")
set(FBSTRING_FileExtents "")
if ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "Chimera Web Plugin")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "Chimera Web Plugin_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )
set(FBSTRING_MIMEType "application/x-chimera-plugin")

# Uncomment this next line if you're not planning on your plugin doing
# any drawing:

#set (FB_GUI_DISABLED 1)

# Mac plugin settings. If your plugin does not draw, set these all to 0
set(FBMAC_USE_QUICKDRAW 0)
set(FBMAC_USE_CARBON 1)
set(FBMAC_USE_COCOA 1)
set(FBMAC_USE_COREGRAPHICS 1)
set(FBMAC_USE_COREANIMATION 0)
set(FBMAC_USE_INVALIDATINGCOREANIMATION 0)

# If you want to register per-machine on Windows, uncomment this line
#set (FB_ATLREG_MACHINEWIDE 1)
