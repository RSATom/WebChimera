#include "Chimera_Win.h"

#include <QtCore/qglobal.h>
#include <QQmlContext.h>
#include <QtQml/QQml.h>
#include <QAbstractNativeEventFilter>
#include <QScreen>

#include "QmlVlc/QmlVlcSurfacePlayerProxy.h"

extern std::string g_dllPath;

////////////////////////////////////////////////////////////////////////////////
//EraseBkgndEater class
////////////////////////////////////////////////////////////////////////////////
//implements workaround of incorrect WM_ERASEBKGND message handling ( at least on Qt 5.3.2 )
class EraseBkgndEater : public QAbstractNativeEventFilter
{
public:
    bool nativeEventFilter( const QByteArray&, void* message, long* result ) override
    {
        MSG* winMessage = (MSG*)message;
        if( winMessage->message == WM_ERASEBKGND ) {
            *result = TRUE;
            return true;
        }
        return false;
    }
};

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

    assert( qApp );
    if( qApp )
        qApp->installNativeEventFilter( new EraseBkgndEater );
}

void Chimera_Win::StaticDeinitialize()
{
    OutputDebugString( L"Chimera_Win::StaticDeinitialize()\n" );

#ifndef _DEBUG
    qUnregisterResourceData( 0x01, qtConf_resource_struct,
                             qtConf_resource_name,
                             (const unsigned char*)qtConf_resource_data.data() );
#endif
}

Chimera_Win::Chimera_Win()
{
    OutputDebugString( L"Chimera_Win::Chimera_Win()\n" );

#ifdef QT_STATIC
    qmlProtectModule( "QtQuick", 2 );
    qmlProtectModule( "QtQuick.Layouts", 1 );
#endif
}

Chimera_Win::~Chimera_Win()
{
    OutputDebugString( L"Chimera_Win::~Chimera_Win()\n" );
}

bool Chimera_Win::onWindowAttached( FB::AttachedEvent* evt, FB::PluginWindowWin* w )
{
    m_pluginWindow.reset( QWindow::fromWinId( (WId) w->getHWND() ) );

    vlc_open();

    m_quickViewPtr.reset( new QQuickView( m_pluginWindow.data() ) );
    m_quickViewPtr->setTitle( QStringLiteral( "WebChimera" ) );
    m_quickViewPtr->setResizeMode( QQuickView::SizeRootObjectToView );
    m_quickViewPtr->setFlags( m_quickViewPtr->flags() | Qt::FramelessWindowHint );

    m_quickViewPtr->setColor( get_bgColor() );
    connect( this, &QmlChimera::bgcolorChanged,
             m_quickViewPtr.data(), &QQuickView::setColor );

    QQmlContext* context = m_quickViewPtr->rootContext();
    m_qmlVlcPlayer = new QmlVlcSurfacePlayerProxy( (vlc::player*)this, m_quickViewPtr.data() );
    m_qmlVlcPlayer->classBegin();

    //have to call apply_player_options()
    //after QmlVlcSurfacePlayerProxy::classBegin
    //to allow attach Proxy's vmem to plugin before play
    apply_player_options();

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
    if( m_quickViewPtr && !is_fullscreen() ) {
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

bool Chimera_Win::is_fullscreen()
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

void Chimera_Win::set_fullscreen( bool fs )
{
    if( m_quickViewPtr && m_pluginWindow ) {
        if( fs && !is_fullscreen() ) {
            QScreen* screen = ScreenFromWindow( static_cast<FB::PluginWindowWin*>( GetWindow() ) );
            m_quickViewPtr->hide();
            m_quickViewPtr->setParent( 0 );
            m_quickViewPtr->setScreen( screen );
            m_quickViewPtr->setGeometry( screen->geometry() );
            m_quickViewPtr->showFullScreen();
            Q_EMIT fullscreenChanged( true );
        } else if( !fs && is_fullscreen() ) {
            m_quickViewPtr->showNormal();
            m_quickViewPtr->setParent( m_pluginWindow.data() );
            onWindowResized( 0, static_cast<FB::PluginWindowWin*>( GetWindow() ) );
            m_quickViewPtr->requestActivate();
            Q_EMIT fullscreenChanged( false );
        }
    }
}
