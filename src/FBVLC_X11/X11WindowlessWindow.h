/*****************************************************************************
 * Copyright (c) 2014 Sergey Radionov <rsatom_gmail.com>
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

#include <NpapiBrowserHost.h>
#include <PluginWindow.h>

#include "X11.h"

class X11WindowlessWindow : public FB::PluginWindow
{
public:
    X11WindowlessWindow( const FB::Npapi::NpapiBrowserHostPtr&, Display*,
                         Colormap, Window );

    virtual void InvalidateWindow() const;

    virtual FB::Rect getWindowPosition() const;
    virtual uint32_t getWindowWidth() const;
    virtual uint32_t getWindowHeight() const;
    void setWindowPosition( const FB::Rect& );

    virtual FB::Rect getWindowClipping() const;
    void setWindowClipping( const FB::Rect& );

    Display* getDisplay() const { return m_display; }
    Colormap getColormap() const { return m_colormap; }
    Window getBrowserWindow() const { return m_browserWindow; }

private:
    FB::Npapi::NpapiBrowserHostPtr m_npHost;
    Display* m_display;
    Colormap m_colormap;
    Window m_browserWindow;

    FB::Rect m_position;
    FB::Rect m_clipping;
};

inline FB::Rect makeRect( int32_t x, int32_t y, uint32_t width, uint32_t height )
{
    FB::Rect rect = { y, x, y + height, x + width };
    return rect;
}

inline uint32_t rectWidth( const FB::Rect& r )
{
    return r.right > r.left ? r.right - r.left : r.left - r.right;
}

inline uint32_t rectHeight( const FB::Rect& r )
{
    return r.bottom > r.top ? r.bottom - r.top : r.top - r.bottom;
}

inline bool operator== ( const FB::Rect& x, const FB::Rect& y )
{
    return x.left == y.left && x.top == y.top &&
           x.right == y.right && x.bottom == y.bottom;
}

inline bool operator!= ( const FB::Rect& x, const FB::Rect& y )
{
    return x.left != y.left || x.top != y.top ||
           x.right != y.right || x.bottom == y.bottom;
}
