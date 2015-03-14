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

#include "Chimera_Win.h"

#include <QQmlContext.h>
#include <QtQml/QQml.h>
#include <QAbstractNativeEventFilter>
#include <QScreen>

#include "QmlVlc/QmlVlcSurfacePlayerProxy.h"

#include "QtConf.h"

extern std::string g_dllPath;

////////////////////////////////////////////////////////////////////////////////
//ChimeraQuickView class
////////////////////////////////////////////////////////////////////////////////
class ChimeraQuickView : public QQuickView
{
public:
    ChimeraQuickView( QWindow* parent )
        : QQuickView( parent ) {}

protected:
    bool nativeEvent( const QByteArray&, void* message, long* result ) override;
};

bool ChimeraQuickView::nativeEvent( const QByteArray&, void* message, long* result )
{
    MSG* winMessage = reinterpret_cast<MSG*>( message );
    switch( winMessage->message ) {
        case WM_MOUSEACTIVATE:
            //always activate QuickView on mouse click
            *result = MA_ACTIVATE;
            break;
        case WM_ERASEBKGND:
            //workaround of incorrect WM_ERASEBKGND message handling ( at least on Qt 5.3.2 )
            *result = TRUE;
            break;
        default:
            return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//Chimera_Win class
////////////////////////////////////////////////////////////////////////////////
void Chimera_Win::StaticInitialize()
{
    OutputDebugString( L"Chimera_Win::StaticInitialize()\n" );

#ifndef _DEBUG
    InitQtConf( g_dllPath + "/../" );
#endif

    QmlChimera::StaticInitialize();
}

void Chimera_Win::StaticDeinitialize()
{
    OutputDebugString( L"Chimera_Win::StaticDeinitialize()\n" );
}

Chimera_Win::Chimera_Win()
{
    OutputDebugString( L"Chimera_Win::Chimera_Win()\n" );
}

Chimera_Win::~Chimera_Win()
{
    OutputDebugString( L"Chimera_Win::~Chimera_Win()\n" );
}

bool Chimera_Win::onWindowAttached( FB::AttachedEvent* evt, FB::PluginWindowWin* w )
{
    m_pluginWindow.reset( QWindow::fromWinId( (WId) w->getHWND() ) );

    vlcOpen();

    m_quickViewPtr.reset( new ChimeraQuickView( m_pluginWindow.data() ) );
    m_quickViewPtr->setTitle( QStringLiteral( "WebChimera" ) );
    m_quickViewPtr->setResizeMode( QQuickView::SizeRootObjectToView );
    m_quickViewPtr->setFlags( m_quickViewPtr->flags() | Qt::FramelessWindowHint );

    m_quickViewPtr->setColor( get_bgColor() );
    connect( this, &QmlChimera::bgcolorChanged,
             m_quickViewPtr.data(), &QQuickView::setColor );

    QQmlContext* context = m_quickViewPtr->rootContext();
    m_qmlVlcPlayer = new QmlVlcSurfacePlayerProxy( (vlc::player*)this, m_quickViewPtr.data() );
    m_qmlVlcPlayer->classBegin();

    //have to call applyPlayerOptions()
    //after QmlVlcSurfacePlayerProxy::classBegin
    //to allow attach Proxy's vmem to plugin before play
    applyPlayerOptions();

    setQml();

    //simulate resize
    onWindowResized( 0, w );

    return false;
}

bool Chimera_Win::onWindowDetached( FB::DetachedEvent*, FB::PluginWindowWin* )
{
    m_quickViewPtr.reset();
    m_pluginWindow.reset();

    return false;
}

bool Chimera_Win::onWindowResized( FB::ResizedEvent*, FB::PluginWindowWin* w )
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

bool Chimera_Win::onWindowsEvent( FB::WindowsEvent* event, FB::PluginWindowWin* w )
{
    if( WM_SETCURSOR == event->uMsg ) {
        event->lRes = FALSE; //allow change cursor by child windows
        return true;
    }

    return false;
}

bool Chimera_Win::isFullscreen()
{
    if( m_quickViewPtr )
        return 0 != ( m_quickViewPtr->visibility() & QWindow::FullScreen );

    return false;
}

QScreen* ScreenFromWindow( FB::PluginWindowWin* w )
{
    POINT topLeft = { 0, 0 };
    ClientToScreen( w->getHWND(), &topLeft );

    QRect winRect = QRect( 0, 0, w->getWindowWidth(), w->getWindowHeight() );
    winRect.moveTo( topLeft.x, topLeft.y );
    QPoint winCenter = winRect.center();

    QList<QScreen*> screens = QGuiApplication::screens();
    QScreen* bestScreen = QGuiApplication::primaryScreen();
    unsigned intersectedArea = 0;
    for( auto* screen : screens ) {
        QRect screenRect = screen->geometry();
        if( screenRect.contains( winCenter ) ) {
            return screen;
        }  else {
            QRect ir = screenRect.intersected( winRect );
            unsigned ia = ir.width() * ir.height();
            if( ir.isValid() && ia > intersectedArea )
                bestScreen = screen;
        }
    }

    return bestScreen;
}

void Chimera_Win::setFullscreen( bool fs )
{
    if( m_quickViewPtr && m_pluginWindow ) {
        if( fs && !isFullscreen() ) {
            QScreen* screen = ScreenFromWindow( static_cast<FB::PluginWindowWin*>( GetWindow() ) );
            m_quickViewPtr->hide();
            m_quickViewPtr->setParent( 0 );
            m_quickViewPtr->setScreen( screen );
            m_quickViewPtr->setGeometry( screen->geometry() );
            m_quickViewPtr->showFullScreen();
            Q_EMIT fullscreenChanged( true );
        } else if( !fs && isFullscreen() ) {
            m_quickViewPtr->showNormal();
            m_quickViewPtr->setParent( m_pluginWindow.data() );
            onWindowResized( 0, static_cast<FB::PluginWindowWin*>( GetWindow() ) );
            m_quickViewPtr->requestActivate();
            Q_EMIT fullscreenChanged( false );
        }
    }
}

void Chimera_Win::onMediaPlayerPlaying()
{
    QuickViewChimera::onMediaPlayerPlaying();

    ::SetThreadExecutionState( ES_CONTINUOUS | ES_DISPLAY_REQUIRED );
}

void Chimera_Win::onMediaPlayerNotPlaying()
{
    QuickViewChimera::onMediaPlayerNotPlaying();

    ::SetThreadExecutionState( ES_CONTINUOUS );
}
