/*****************************************************************************
 * Copyright © 2014-2015 Sergey Radionov <rsatom_gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#pragma once

#include <QScopedPointer>

#include <PluginWindowMacCA.h>
#include <PluginEvents/KeyboardEvents.h>

#include "../QmlChimera.h"

class FboQuickView; //#include <QuickLayer/FboQuickView.h>

////////////////////////////////////////////////////////////////////////////////
//Cimera_Mac class
////////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR( Chimera_Mac )
class Chimera_Mac: public QmlChimera
{
    Q_OBJECT
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
    EVENTTYPE_CASE( FB::ResizedEvent, onWindowResized, FB::PluginWindowMacCA )
    //EVENTTYPE_CASE( FB::WindowsEvent, onWindowsEvent, FB::PluginWindowMac )
    EVENTTYPE_CASE( FB::MouseDownEvent, onMouseDown, FB::PluginWindowMacCA )
    EVENTTYPE_CASE( FB::MouseUpEvent, onMouseUp, FB::PluginWindowMacCA )
    EVENTTYPE_CASE( FB::MouseDoubleClickEvent, onMouseDblClick, FB::PluginWindowMacCA )
    EVENTTYPE_CASE( FB::MouseMoveEvent, onMouseMove, FB::PluginWindowMacCA )
    EVENTTYPE_CASE( FB::KeyDownEvent, onKeyDown, FB::PluginWindowMacCA )
    EVENTTYPE_CASE( FB::KeyUpEvent, onKeyUp, FB::PluginWindowMacCA )
    PLUGIN_EVENT_MAP_CASCADE( QmlChimera )
    END_PLUGIN_EVENT_MAP()

private:
    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    bool onWindowAttached( FB::AttachedEvent*, FB::PluginWindowMacCA* );
    bool onWindowDetached( FB::DetachedEvent*, FB::PluginWindowMacCA* );
    //bool onWindowDetached( FB::DetachedEvent*, FB::PluginWindowMac* );
    bool onWindowResized( FB::ResizedEvent*, FB::PluginWindowMacCA* );
    //bool onWindowsEvent( FB::WindowsEvent*, FB::PluginWindowMac* );
    bool onMouseDown( FB::MouseDownEvent*, FB::PluginWindowMacCA* );
    bool onMouseUp( FB::MouseUpEvent*, FB::PluginWindowMacCA* );
    bool onMouseDblClick( FB::MouseDoubleClickEvent*, FB::PluginWindowMacCA* );
    bool onMouseEnter( FB::MouseEnteredEvent*, FB::PluginWindowMacCA* );
    bool onMouseLeave( FB::MouseExitedEvent*, FB::PluginWindowMacCA* );
    bool onMouseMove( FB::MouseMoveEvent*, FB::PluginWindowMacCA* );
    bool onKeyDown( FB::KeyDownEvent*, FB::PluginWindowMacCA* );
    bool onKeyUp( FB::KeyUpEvent*, FB::PluginWindowMacCA* );
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

public:
    bool isFullscreen() override;
    void setFullscreen( bool fs ) override;

protected:
    void on_option_change( vlc_player_option_e );

private:
    void setQml();
    void cleanup();

private Q_SLOTS:
    void quickViewStatusChanged();

private:
    struct Private;
    std::unique_ptr<Private> m_p;
};
