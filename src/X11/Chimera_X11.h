/*****************************************************************************
 * Copyright © 2014 Sergey Radionov <rsatom_gmail.com>
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

#include "PluginWindowX11.h"

#include "PluginEvents/X11Event.h"

#include "../QuickViewChimera.h"

////////////////////////////////////////////////////////////////////////////////
//FBVLC_X11 class
////////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR( Chimera_X11 )
class Chimera_X11: public QuickViewChimera
{
public:
    Chimera_X11();
    virtual ~Chimera_X11();

    BEGIN_PLUGIN_EVENT_MAP()
        EVENTTYPE_CASE( FB::AttachedEvent, onWindowAttached, FB::PluginWindowX11 )
        EVENTTYPE_CASE( FB::DetachedEvent, onWindowDetached, FB::PluginWindowX11 )
        EVENTTYPE_CASE( FB::ResizedEvent, onWindowResized, FB::PluginWindowX11 )
        EVENTTYPE_CASE( FB::X11Event, onX11Event, FB::PluginWindowX11 )
        PLUGIN_EVENT_MAP_CASCADE( Chimera )
    END_PLUGIN_EVENT_MAP()

private:
    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    bool onWindowAttached( FB::AttachedEvent*, FB::PluginWindowX11* );
    bool onWindowDetached( FB::DetachedEvent*, FB::PluginWindowX11* );
    bool onWindowResized( FB::ResizedEvent*, FB::PluginWindowX11* );
    bool onX11Event( FB::X11Event*, FB::PluginWindowX11* );
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

private:
    QScopedPointer<QWindow> m_pluginWindow;
};
