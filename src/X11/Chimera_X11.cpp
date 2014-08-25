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
