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

//#include <boost/algorithm/string/replace.hpp>

//#include <QtCore/qglobal.h>
//#include <QtPlugin>
#include <QQmlContext>
//#include <QtQml/QQml.h>
//#include <QtEndian>
#include <QtWidgets/QWidget>

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

bool Chimera_X11::onWindowAttached( FB::AttachedEvent *evt, FB::PluginWindowX11* w )
{
    vlc_open();

    QWindow* browserWindow = QWindow::fromWinId( w->getWindow() );//FIXME, memory leak
    m_quickViewPtr.reset( new QQuickView( browserWindow ) );

    m_quickViewPtr->setResizeMode( QQuickView::SizeRootObjectToView );

    QQmlContext* context = m_quickViewPtr->rootContext();
    m_qmlVlcPlayer = new QmlVlcSurfacePlayerProxy( (vlc::player*)this, m_quickViewPtr.data() );
    m_qmlVlcPlayer->classBegin();
    context->setContextProperty( "vlcPlayer", QVariant::fromValue( m_qmlVlcPlayer ) );

    process_startup_options();

    setQml();

    m_quickViewPtr->setX( 0 );
    m_quickViewPtr->setY( 0 );

    m_quickViewPtr->resize( w->getWindowWidth(), w->getWindowHeight() );
    m_quickViewPtr->show();

    return false;
}

bool Chimera_X11::onWindowResized( FB::ResizedEvent *evt, FB::PluginWindowX11* w )
{
    if( m_quickViewPtr ) {
        m_quickViewPtr->resize( w->getWindowWidth(), w->getWindowHeight() );
    }

    return false;
}
