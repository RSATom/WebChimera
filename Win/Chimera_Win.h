#ifndef H_CHIMERA_WIN
#define H_CHIMERA_WIN

#include "PluginWindowWin.h"

#include "../Chimera.h"

////////////////////////////////////////////////////////////////////////////////
//FBVLC_Win class
////////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR(Chimera_Win)
class Chimera_Win: public Chimera
{
public:
    Chimera_Win();
    virtual ~Chimera_Win();

    BEGIN_PLUGIN_EVENT_MAP()
        EVENTTYPE_CASE(FB::AttachedEvent, onWindowAttached, FB::PluginWindowWin)
        EVENTTYPE_CASE(FB::DetachedEvent, onWindowDetached, FB::PluginWindowWin)
        EVENTTYPE_CASE(FB::ResizedEvent, onWindowResized, FB::PluginWindowWin)
        PLUGIN_EVENT_MAP_CASCADE(Chimera)
    END_PLUGIN_EVENT_MAP()

private:
    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    bool onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindowWin *);
    bool onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindowWin *);
    bool onWindowResized(FB::ResizedEvent *evt, FB::PluginWindowWin *);
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

public:
    bool is_fullscreen();
    void set_fullscreen(bool fs);
    void toggle_fullscreen();
};

#endif //H_CHIMERA_WIN
