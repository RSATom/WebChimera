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

#include "PluginEvents/WindowsEvent.h"

#include "PluginWindowWin.h"

#include "../QuickViewChimera.h"

////////////////////////////////////////////////////////////////////////////////
//Chimera_Win class
////////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR( Chimera_Win )
class Chimera_Win: public QuickViewChimera
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
    bool isFullscreen() override;
    void setFullscreen( bool fs ) override;

protected:
    void onMediaPlayerPlaying() override;
    void onMediaPlayerNotPlaying() override;

private:
    QScopedPointer<QWindow> m_pluginWindow;
};
