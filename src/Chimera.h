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

#include "PluginWindow.h"
#include "PluginEvents/MouseEvents.h"
#include "PluginEvents/AttachedEvent.h"
#include "PluginEvents/DrawingEvents.h"

#include "PluginCore.h"

#include <libvlc_wrapper/vlc_player.h>

#include "vlc_player_options.h"

FB_FORWARD_PTR( Chimera )
class Chimera
    : public FB::PluginCore,
      protected vlc_player,
      protected vlc_player_options,
      private vlc::media_player_events_callback

{
public:
    static void StaticInitialize() {}
    static void StaticDeinitialize() {}

public:
    Chimera();
    virtual ~Chimera();

protected:
    void onPluginReady();
    void shutdown();
    // If you want your plugin to always be windowless, set this to true
    // If you want your plugin to be optionally windowless based on the
    // value of the "windowless" param tag, remove this method or return
    // FB::PluginCore::isWindowless()
    virtual bool isWindowless() { return false; }

    BEGIN_PLUGIN_EVENT_MAP()
    END_PLUGIN_EVENT_MAP()

private:
    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

public:
    libvlc_instance_t* getLibVlc()
        { return m_libvlc; }
    vlc_player& get_player()
        { return *static_cast<vlc_player*>( this ); }
    vlc_player_options& get_options()
        { return *static_cast<vlc_player_options*>( this ); }
    const vlc_player_options& get_options() const
        { return *static_cast<const vlc_player_options*>( this ); }

    int addPlaylistItem( const std::string& mrl );
    int addPlaylistItem( const std::string& mrl,
                           const std::vector<std::string>& options );

public:
    virtual bool isFullscreen() { return false; }
    virtual void setFullscreen( bool fs ) {}
    virtual void toggleFullscreen()
        { setFullscreen( !isFullscreen() ); }

private:
    std::string detectHttpProxy( const std::string& mrl ) const;

protected:
    const FB::variant& getParamVariant( const std::string& key ) const;

    virtual void loadStartupOptions();
    void vlcOpen();
    void applyPlayerOptions();
    void vlcClose();

    virtual bool isOptionTrusted( const std::string& /*option*/ )
        { return false; }

    virtual void on_option_change( vlc_player_option_e );

    virtual void onMediaPlayerPlaying();
    virtual void onMediaPlayerNotPlaying();

private:
    void loadLibvlcOptions();

    void media_player_event( const libvlc_event_t* e ) override;

private:
    libvlc_instance_t* m_libvlc;
    bool m_forceMute;
};
