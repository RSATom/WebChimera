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

#pragma once

#include "PluginWindowWin.h"
#include "PluginWindowlessWin.h"

#include <libvlc_wrapper/vlc_vmem.h>

#include "win32_fullscreen.h"

#include "Chimera.h"

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
class FBVLC_Win
    : public Chimera,
      protected vlc::vmem
{
public:
    FBVLC_Win();
    virtual ~FBVLC_Win();

    BEGIN_PLUGIN_EVENT_MAP()
        EVENTTYPE_CASE( FB::AttachedEvent, onWindowAttached, FB::PluginWindowlessWin )
        EVENTTYPE_CASE( FB::DetachedEvent, onWindowDetached, FB::PluginWindowlessWin )
        EVENTTYPE_CASE( FB::ResizedEvent, onWindowResized, FB::PluginWindowlessWin )
        EVENTTYPE_CASE( FB::RefreshEvent, onRefreshEvent, FB::PluginWindowlessWin )

        EVENTTYPE_CASE( FB::AttachedEvent, onWindowAttached, FB::PluginWindowWin )
        EVENTTYPE_CASE( FB::DetachedEvent, onWindowDetached, FB::PluginWindowWin )
        EVENTTYPE_CASE( FB::ResizedEvent, onWindowResized, FB::PluginWindowWin )
        PLUGIN_EVENT_MAP_CASCADE( Chimera )
    END_PLUGIN_EVENT_MAP()

private:
    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    bool onWindowAttached( FB::AttachedEvent*, FB::PluginWindowlessWin* );
    bool onWindowDetached( FB::DetachedEvent*, FB::PluginWindowlessWin* );
    bool onWindowResized( FB::ResizedEvent*, FB::PluginWindowlessWin* );
    bool onRefreshEvent( FB::RefreshEvent*, FB::PluginWindowlessWin* );

    bool onWindowAttached( FB::AttachedEvent*, FB::PluginWindowWin* );
    bool onWindowDetached( FB::DetachedEvent*, FB::PluginWindowWin* );
    bool onWindowResized( FB::ResizedEvent*, FB::PluginWindowWin* );
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

    void update_window();

public:
    bool isFullscreen() override;
    void setFullscreen( bool fs ) override;

protected:
    bool isWindowless() override
        { return FB::PluginCore::isWindowless(); }

    FB::JSAPIPtr createJSAPI() override;

    void loadStartupOptions() override;

    void on_option_change( vlc_player_option_e ) override;
    void on_frame_ready( const std::vector<char>* ) override;
    void on_frame_cleanup() override;

    void onMediaPlayerPlaying() override;
    void onMediaPlayerNotPlaying() override;

private:
    bool m_use_native_scaling;
//for windowless mode
    HBRUSH m_hBgBrush;
//for windowed mode
    std::auto_ptr<WindowedWM> m_wm;

    boost::mutex m_frame_guard;
    const std::vector<char>* m_frame_buf;
};
