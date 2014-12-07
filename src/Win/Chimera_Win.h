#ifndef H_CHIMERA_WIN
#define H_CHIMERA_WIN

#include "PluginEvents/WindowsEvent.h"

#include "PluginWindowWin.h"

#include "../QmlChimera.h"

////////////////////////////////////////////////////////////////////////////////
//Chimera_Win class
////////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR( Chimera_Win )
class Chimera_Win: public QmlChimera
{
public:
    static void StaticInitialize();
    static void StaticDeinitialize();

public:
    Chimera_Win();
    virtual ~Chimera_Win();

    BEGIN_PLUGIN_EVENT_MAP()
        EVENTTYPE_CASE( FB::AttachedEvent, onWindowAttached, FB::PluginWindowWin )
        EVENTTYPE_CASE( FB::DetachedEvent, onWindowDetached, FB::PluginWindowWin )
        EVENTTYPE_CASE( FB::ResizedEvent, onWindowResized, FB::PluginWindowWin )
        EVENTTYPE_CASE( FB::WindowsEvent, onWindowsEvent, FB::PluginWindowWin )
        PLUGIN_EVENT_MAP_CASCADE( QmlChimera )
    END_PLUGIN_EVENT_MAP()

private:
    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    bool onWindowAttached( FB::AttachedEvent*, FB::PluginWindowWin* );
    bool onWindowDetached( FB::DetachedEvent*, FB::PluginWindowWin* );
    bool onWindowResized( FB::ResizedEvent*, FB::PluginWindowWin* );
    bool onWindowsEvent( FB::WindowsEvent*, FB::PluginWindowWin* );
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

public:
    bool is_fullscreen();
    void set_fullscreen( bool fs );

private:
    QScopedPointer<QWindow> m_pluginWindow;
};

#endif //H_CHIMERA_WIN
