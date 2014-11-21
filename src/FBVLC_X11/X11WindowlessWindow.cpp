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

#include "X11WindowlessWindow.h"

#include <PluginEvents/DrawingEvents.h>

X11WindowlessWindow::X11WindowlessWindow( const FB::Npapi::NpapiBrowserHostPtr& host,
                                          Display* display,
                                          Colormap colormap,
                                          Window browserWindow )
    : m_npHost( host ), m_display( display ),
      m_colormap( colormap ), m_browserWindow( browserWindow )
{
    m_position = m_clipping = makeRect( 0, 0, 0, 0 );
}

void X11WindowlessWindow::InvalidateWindow() const
{
    NPRect rect = { 0, 0, getWindowHeight(), getWindowWidth() };
    if( !m_npHost->isMainThread() )
        m_npHost->ScheduleOnMainThread(
            m_npHost, boost::bind( &FB::Npapi::NpapiBrowserHost::InvalidateRect2,
                                   m_npHost, rect ) );
    else
        m_npHost->InvalidateRect( &rect );
}

FB::Rect X11WindowlessWindow::getWindowPosition() const
{
    return m_position;
}

uint32_t X11WindowlessWindow::getWindowWidth() const
{
    return rectWidth( m_position );
}

uint32_t X11WindowlessWindow::getWindowHeight() const
{
    return rectHeight( m_position );
}

void X11WindowlessWindow::setWindowPosition( const FB::Rect& position )
{
    if( position != m_position ) {
        m_position = position;
        FB::ResizedEvent evt;
        SendEvent(&evt);
    }
}

FB::Rect X11WindowlessWindow::getWindowClipping() const
{
    return m_clipping;
}

void X11WindowlessWindow::setWindowClipping( const FB::Rect& clipping )
{
    if( clipping != m_clipping ) {
        m_clipping = clipping;
        FB::ClipChangedEvent evt;
        SendEvent(&evt);
    }
}
