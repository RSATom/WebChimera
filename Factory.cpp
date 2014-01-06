/**********************************************************\ 
 
 Auto-generated Factory.cpp
 
 This file contains the auto-generated factory methods 
 for the Chimera project
 
\**********************************************************/

#include "FactoryBase.h"
#ifdef FB_WIN
#include "Win/Chimera_Win.h"
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
#ifdef FB_WIN
        return boost::make_shared<Chimera_Win>();
#else
        return boost::make_shared<Chimera>();
#endif
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @see FB::FactoryBase::globalPluginInitialize
    ///////////////////////////////////////////////////////////////////////////////
    void globalPluginInitialize()
    {
        Chimera::StaticInitialize();
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @see FB::FactoryBase::globalPluginDeinitialize
    ///////////////////////////////////////////////////////////////////////////////
    void globalPluginDeinitialize()
    {
        Chimera::StaticDeinitialize();
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

