/*****************************************************************************
 * Copyright © 2002-2011 VideoLAN and VLC authors
 * $Id$
 *
 * Authors: Sergey Radionov <rsatom_gmail.com>
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

#ifndef _VLC_PLAYER_OPTIONS_H_
#define _VLC_PLAYER_OPTIONS_H_

#include <string>
#include <stdint.h>

enum vlc_player_option_e
{
    po_autoplay,
    po_enable_fullscreen,
    po_bg_text,
    po_bg_color,
    po_use_proxy,
    po_qml_source,
    po_qml,
};

class vlc_player_options
{
public:
    vlc_player_options()
        : _autoplay( true ), _enable_fullscreen( true ),
          _use_proxy( true ), _bg_color( /*black*/"#000000" )
    {}

    void set_autoplay( bool ap ) {
        _autoplay = ap;
        on_option_change( po_autoplay );
    }
    bool get_autoplay() const
        { return _autoplay; }

    void set_enable_fs( bool ef ){
        _enable_fullscreen = ef;
        on_option_change( po_enable_fullscreen );
    }
    bool get_enable_fs() const
        { return _enable_fullscreen; }

    void set_bg_text( const std::string& bt ) {
        _bg_text = bt;
        on_option_change( po_bg_text );
    }
    const std::string& get_bg_text() const {
        return _bg_text;
    }

    void set_bg_color( const std::string& bc ){
        _bg_color = bc;
        on_option_change( po_bg_color );
    }
    const std::string& get_bg_color() const {
        return _bg_color;
    }

    void set_use_proxy( bool up ) {
        _use_proxy = up;
        on_option_change( po_use_proxy );
    }
    bool get_use_proxy() const
        { return _use_proxy; }

    void set_qml_source( const std::string& src ) {
        _qml_source = src;
        on_option_change( po_qml_source );
    }
    const std::string& get_qml_source() const
        { return _qml_source; }

    void set_qml( const std::string& qml ) {
        _qml = qml;
        on_option_change( po_qml );
    }
    const std::string& get_qml() const
        { return _qml; }

protected:
    virtual void on_option_change( vlc_player_option_e ) {}

private:
    bool        _autoplay;
    bool        _enable_fullscreen;
    bool        _use_proxy;
    std::string _bg_text;
    // http://www.w3.org/TR/SVG/types.html#ColorKeywords
    // http://www.w3.org/TR/SVG/types.html#DataTypeColor
    std::string _bg_color;
    std::string _qml_source;
    std::string _qml;
};

#endif //_VLC_PLAYER_OPTIONS_H_
