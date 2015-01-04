#pragma once

#include <QScopedPointer>

#include "PluginWindowMacCA.h"

#include "../QmlChimera.h"

class FboQuickView; //#include <QuickLayer/FboQuickView.h>

////////////////////////////////////////////////////////////////////////////////
//Cimera_Mac class
////////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR( Chimera_Mac )
class Chimera_Mac: public QmlChimera
{
public:
    static void StaticInitialize();
    static void StaticDeinitialize();

public:
    Chimera_Mac();
    virtual ~Chimera_Mac();

    BEGIN_PLUGIN_EVENT_MAP()
    EVENTTYPE_CASE( FB::MouseEnteredEvent, onMouseEnter, FB::PluginWindowMacCA )
    EVENTTYPE_CASE( FB::MouseExitedEvent, onMouseLeave, FB::PluginWindowMacCA )
    EVENTTYPE_CASE( FB::AttachedEvent, onWindowAttached, FB::PluginWindowMacCA )
    EVENTTYPE_CASE( FB::DetachedEvent, onWindowDetached, FB::PluginWindowMacCA )
    //EVENTTYPE_CASE( FB::DetachedEvent, onWindowDetached, FB::PluginWindowMac )
    //EVENTTYPE_CASE( FB::ResizedEvent, onWindowResized, FB::PluginWindowMac )
    //EVENTTYPE_CASE( FB::WindowsEvent, onWindowsEvent, FB::PluginWindowMac )
    EVENTTYPE_CASE( FB::MouseDownEvent, onMouseDown, FB::PluginWindowMacCA )
    EVENTTYPE_CASE( FB::MouseUpEvent, onMouseUp, FB::PluginWindowMacCA )
    EVENTTYPE_CASE( FB::MouseMoveEvent, onMouseMove, FB::PluginWindowMacCA )
    PLUGIN_EVENT_MAP_CASCADE( QmlChimera )
    END_PLUGIN_EVENT_MAP()

private:
    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    bool onWindowAttached( FB::AttachedEvent*, FB::PluginWindowMacCA* );
    bool onWindowDetached( FB::DetachedEvent*, FB::PluginWindowMacCA* );
    //bool onWindowDetached( FB::DetachedEvent*, FB::PluginWindowMac* );
    //bool onWindowResized( FB::ResizedEvent*, FB::PluginWindowMac* );
    //bool onWindowsEvent( FB::WindowsEvent*, FB::PluginWindowMac* );
    bool onMouseDown( FB::MouseDownEvent*, FB::PluginWindowMacCA* );
    bool onMouseUp( FB::MouseUpEvent*, FB::PluginWindowMacCA* );
    bool onMouseEnter( FB::MouseEnteredEvent*, FB::PluginWindowMacCA* );
    bool onMouseLeave( FB::MouseExitedEvent*, FB::PluginWindowMacCA* );
    bool onMouseMove( FB::MouseMoveEvent*, FB::PluginWindowMacCA* );
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

protected:
    void on_option_change( vlc_player_option_e );

private:
    void setQml();
    void cleanup();

private Q_SLOTS:
    void quickViewStatusChanged();

private:
    QScopedPointer<FboQuickView> m_quickViewPtr;
    void* m_quickLayer;
};
