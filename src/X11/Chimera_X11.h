#pragma once

#include "PluginWindowX11.h"

#include "../Chimera.h"

////////////////////////////////////////////////////////////////////////////////
//FBVLC_X11 class
////////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR( Chimera_X11 )
class Chimera_X11: public Chimera
{
public:
    Chimera_X11();
    virtual ~Chimera_X11();

    BEGIN_PLUGIN_EVENT_MAP()
        EVENTTYPE_CASE( FB::AttachedEvent, onWindowAttached, FB::PluginWindowX11 )
        EVENTTYPE_CASE( FB::ResizedEvent, onWindowResized, FB::PluginWindowX11 )
        PLUGIN_EVENT_MAP_CASCADE( Chimera )
    END_PLUGIN_EVENT_MAP()

private:
    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    bool onWindowAttached( FB::AttachedEvent*, FB::PluginWindowX11* );
    bool onWindowResized( FB::ResizedEvent*, FB::PluginWindowX11* );
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/
};
