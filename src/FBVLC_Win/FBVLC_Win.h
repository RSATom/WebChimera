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

#ifndef H_FBVLCPLUGIN_WIN
#define H_FBVLCPLUGIN_WIN

#include "../FBVLC.h"
#include "win32_fullscreen.h"

////////////////////////////////////////////////////////////////////////////////
//WindowedWM class
////////////////////////////////////////////////////////////////////////////////
class WindowedWM : public VLCWindowsManager
{
public:
    WindowedWM( HMODULE hDllModule, vlc_player_options* po );

private:
    VLCViewResources m_rc;
};

////////////////////////////////////////////////////////////////////////////////
//FBVLC_Win class
////////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR( FBVLC_Win )
class FBVLC_Win: public FBVLC
{
public:
    FBVLC_Win();
    virtual ~FBVLC_Win();

    BEGIN_PLUGIN_EVENT_MAP()
        EVENTTYPE_CASE( FB::RefreshEvent, onRefreshEvent, FB::PluginWindowlessWin )

        EVENTTYPE_CASE( FB::AttachedEvent, onWindowAttached, FB::PluginWindowWin )
        EVENTTYPE_CASE( FB::DetachedEvent, onWindowDetached, FB::PluginWindowWin )
        EVENTTYPE_CASE( FB::ResizedEvent, onWindowResized, FB::PluginWindowWin )
        PLUGIN_EVENT_MAP_CASCADE( FBVLC )
    END_PLUGIN_EVENT_MAP()

private:
    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    bool onRefreshEvent( FB::RefreshEvent*, FB::PluginWindowlessWin* );

    bool onWindowAttached( FB::AttachedEvent*, FB::PluginWindowWin* );
    bool onWindowDetached( FB::DetachedEvent*, FB::PluginWindowWin* );
    bool onWindowResized( FB::ResizedEvent*, FB::PluginWindowWin* );
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

    void update_window();

public:
    bool is_fullscreen();
    void set_fullscreen( bool fs );
    void toggle_fullscreen();

protected:
    virtual void on_option_change( vlc_player_option_e );
    virtual void on_frame_ready( const std::vector<char>* );
    virtual void on_frame_cleanup();

protected:
//for windowless mode
    HBRUSH m_hBgBrush;
//for windowed mode
    std::auto_ptr<WindowedWM> m_wm;

private:
    boost::mutex m_frame_guard;
    const std::vector<char>* m_frame_buf;
};

#endif//H_FBVLCPLUGIN_WIN
