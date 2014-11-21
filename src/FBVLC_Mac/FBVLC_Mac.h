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

 #ifndef H_FBVLCPLUGIN_MAC
#define H_FBVLCPLUGIN_MAC

#include <boost/thread/mutex.hpp>

#include "../FBVLC.h"

#include "PluginEvents/MacEventCocoa.h"
#include "Mac/PluginWindowMac.h"
#include "Mac/PluginWindowMacCG.h"

////////////////////////////////////////////////////////////////////////////////
//FBVLC_Mac class
////////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR( FBVLC_Mac )
class FBVLC_Mac: public FBVLC
{
public:
    FBVLC_Mac();
    virtual ~FBVLC_Mac();

    //mac plugins is always windowless
    virtual bool isWindowless() { return true; }

    BEGIN_PLUGIN_EVENT_MAP()
        EVENTTYPE_CASE( FB::CoreGraphicsDraw, onCoreGraphicsDraw, FB::PluginWindowMacCG )
        PLUGIN_EVENT_MAP_CASCADE( FBVLC )
    END_PLUGIN_EVENT_MAP()

private:
    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    bool onCoreGraphicsDraw( FB::CoreGraphicsDraw*, FB::PluginWindowMacCG* );
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

public:
    virtual bool is_fullscreen() { return false; };
    virtual void set_fullscreen( bool fs ) { };
    virtual void toggle_fullscreen() { };

protected:
    virtual void on_frame_ready( const std::vector<char>* frame_buf );
    virtual void on_frame_cleanup();

protected:
    virtual void on_option_change( vlc_player_option_e );

private:
    void updateBgComponents();

private:
    CGFloat m_bgComponents[4];

    boost::mutex m_frameGuard;
    const std::vector<char>* m_frameBuf;
};

#endif//H_FBVLCPLUGIN_MAC
