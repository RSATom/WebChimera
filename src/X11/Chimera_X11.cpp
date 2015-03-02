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

#include "Chimera_X11.h"

#include "QmlVlc/QmlVlcSurfacePlayerProxy.h"

////////////////////////////////////////////////////////////////////////////////
//Chimera_X11 class
////////////////////////////////////////////////////////////////////////////////
Chimera_X11::Chimera_X11()
{
}

Chimera_X11::~Chimera_X11()
{
}

bool Chimera_X11::onX11Event( FB::X11Event* evt, FB::PluginWindowX11* w )
{
    if( GDK_MAP == evt->m_event->type ) {
        onWindowResized( 0, w );
    }

    return false;
}

bool Chimera_X11::onWindowAttached( FB::AttachedEvent* evt, FB::PluginWindowX11* w )
{
    m_pluginWindow.reset( QWindow::fromWinId( (WId) w->getWindow() ) );

    vlcOpen();

    m_quickViewPtr.reset( new QQuickView( m_pluginWindow.data() ) );
    m_quickViewPtr->setTitle( QStringLiteral( "WebChimera" ) );
    m_quickViewPtr->setResizeMode( QQuickView::SizeRootObjectToView );
    m_quickViewPtr->setFlags( m_quickViewPtr->flags() | Qt::FramelessWindowHint );

    m_quickViewPtr->setColor( get_bgColor() );
    connect( this, &QmlChimera::bgcolorChanged,
             m_quickViewPtr.data(), &QQuickView::setColor );

    m_qmlVlcPlayer = new QmlVlcSurfacePlayerProxy( (vlc::player*)this, m_quickViewPtr.data() );
    m_qmlVlcPlayer->classBegin();

    //have to call applyPlayerOptions()
    //after QmlVlcSurfacePlayerProxy::classBegin
    //to allow attach Proxy's vmem to plugin before play
    applyPlayerOptions();

    //simulate resize
    //onWindowResized( 0, w );

    setQml();

    return false;
}

bool Chimera_X11::onWindowResized( FB::ResizedEvent*, FB::PluginWindowX11* w )
{
    const int newWidth = w->getWindowWidth();
    const int newHeight = w->getWindowHeight();
    if( m_quickViewPtr && !isFullscreen() ) {
        if( newWidth > 0 && newHeight > 0 ) {
            if( !m_quickViewPtr->isVisible() )
                m_quickViewPtr->show();
            m_quickViewPtr->setX( 0 ); m_quickViewPtr->setY( 0 );
            m_quickViewPtr->resize( newWidth, newHeight );
        } else
            m_quickViewPtr->hide();
    }

    return false;
}

bool Chimera_X11::onWindowDetached( FB::DetachedEvent*, FB::PluginWindowX11* )
{
    m_quickViewPtr.reset();
    m_pluginWindow.reset();

    return false;
}
