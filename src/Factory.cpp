/**********************************************************\ 
 
 Auto-generated Factory.cpp
 
 This file contains the auto-generated factory methods 
 for the Chimera project
 
\**********************************************************/

#include "FactoryBase.h"
#if defined( FB_WIN )
#include "Win/Chimera_Win.h"
#include "Win/FBVLC_Win.h"
#elif defined( FB_X11 )
#include "X11/Chimera_X11.h"
#elif defined( FB_MACOSX )
#include "Mac/Chimera_Mac.h"
#else
#include "Chimera.h"
#endif
#include <boost/make_shared.hpp>

class PluginFactory : public FB::FactoryBase
{
public:
    ///////////////////////////////////////////////////////////////////////////////
    /// @fn FB::PluginCorePtr createPlugin(const std::string& mimetype)
    ///
    /// @brief  Creates a plugin object matching the provided mimetype
    ///         If mimetype is empty, returns the default plugin
    ///////////////////////////////////////////////////////////////////////////////
    FB::PluginCorePtr createPlugin( const std::string& mimetype )
    {
#if defined( FB_WIN )
        if( "application/x-fb-vlc" == mimetype ) {
            return boost::make_shared<FBVLC_Win>();
        } else {
#else
        {
#endif
#if defined( FB_WIN )
            return boost::make_shared<Chimera_Win>();
#elif defined( FB_X11 )
            return boost::make_shared<Chimera_X11>();
#elif defined( FB_MACOSX )
            return boost::make_shared<Chimera_Mac>();
#else
            return boost::make_shared<Chimera>();
#endif
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @see FB::FactoryBase::globalPluginInitialize
    ///////////////////////////////////////////////////////////////////////////////
    void globalPluginInitialize()
    {
#if defined( FB_WIN )
        Chimera_Win::StaticInitialize();
#elif defined( FB_X11 )
        return Chimera_X11::StaticInitialize();
#elif defined( FB_MACOSX )
        return Chimera_Mac::StaticInitialize();
#else
        return Chimera::StaticInitialize();
#endif
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @see FB::FactoryBase::globalPluginDeinitialize
    ///////////////////////////////////////////////////////////////////////////////
    void globalPluginDeinitialize()
    {
#ifdef defined( FB_WIN )
        return Chimera_Win::StaticDeinitialize();
#elif defined( FB_MACOSX )
        return Chimera_Mac::StaticDeinitialize();
#else
        return Chimera::StaticDeinitialize();
#endif
    }
};

///////////////////////////////////////////////////////////////////////////////
/// @fn getFactoryInstance()
///
/// @brief  Returns the factory instance for this plugin module
///////////////////////////////////////////////////////////////////////////////
FB::FactoryBasePtr getFactoryInstance()
{
    static boost::shared_ptr<PluginFactory> factory = boost::make_shared<PluginFactory>();
    return factory;
}

