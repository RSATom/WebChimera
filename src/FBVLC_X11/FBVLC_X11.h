/*****************************************************************************
 * Copyright (c) 2012-2014 Sergey Radionov <rsatom_gmail.com>
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

#ifndef H_FBVLCPLUGIN_X11
#define H_FBVLCPLUGIN_X11

#include "../FBVLC.h"

#include "X11/PluginWindowX11.h"

#include "X11WindowlessWindow.h"
#include "X11ExposeEvent.h"

////////////////////////////////////////////////////////////////////////////////
//FBVLC_X11 class
////////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR(FBVLC_X11)
class FBVLC_X11: public FBVLC
{
public:
    FBVLC_X11();
    virtual ~FBVLC_X11();

    //Note: events not routed to FBVLC due to FireBreath limitations
    BEGIN_PLUGIN_EVENT_MAP()
        EVENTTYPE_CASE( FB::AttachedEvent, onWindowAttached, FB::PluginWindowX11 )
        EVENTTYPE_CASE( FB::DetachedEvent, onWindowDetached, FB::PluginWindowX11 )

        EVENTTYPE_CASE( FB::AttachedEvent, onWindowAttached, X11WindowlessWindow )
        EVENTTYPE_CASE( FB::DetachedEvent, onWindowDetached, X11WindowlessWindow )
        EVENTTYPE_CASE( X11ExposeEvent, onExposeEvent, X11WindowlessWindow )
        PLUGIN_EVENT_MAP_CASCADE( FBVLC )
    END_PLUGIN_EVENT_MAP()

    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    bool onWindowAttached( FB::AttachedEvent*, FB::PluginWindowX11* );
    bool onWindowDetached( FB::DetachedEvent*, FB::PluginWindowX11* );

    bool onWindowAttached( FB::AttachedEvent*, X11WindowlessWindow* );
    bool onWindowDetached( FB::DetachedEvent*, X11WindowlessWindow* );
    bool onExposeEvent( X11ExposeEvent*, X11WindowlessWindow* );
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

protected:
    virtual void on_option_change( vlc_player_option_e );

protected:
   virtual void on_frame_ready( const std::vector<char>* );
   virtual void on_frame_cleanup();

private:
    void updateWindow();
    void updateBgColor();
    void fillBackground( xcb_drawable_t drawable,
                         const xcb_rectangle_t* bgRects,
                         uint32_t rectCount );

private:
    xcb_connection_t* m_xcbConnection;
    xcb_gcontext_t m_xcbContextId;

private:
    boost::mutex m_frameGuard;
    const std::vector<char>* m_frameBuf;
};

#endif//H_FBVLCPLUGIN_X11
