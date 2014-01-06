#include "Chimera_Win.h"

#include <QQmlContext.h>

#include "QmlVlc/QmlVlcPlayer.h"

#include <QtPlugin>
#include <QtQml/QQml.h>

Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
Q_IMPORT_PLUGIN(QtQuick2Plugin);
Q_IMPORT_PLUGIN(QMultimediaDeclarativeModule);

////////////////////////////////////////////////////////////////////////////////
//Chimera_Win class
////////////////////////////////////////////////////////////////////////////////
Chimera_Win::Chimera_Win()
{
    qmlProtectModule( "QtQuick", 2 );
    qmlProtectModule( "QtMultimedia", 5);
}

Chimera_Win::~Chimera_Win()
{
}

bool Chimera_Win::onWindowAttached( FB::AttachedEvent *evt, FB::PluginWindowWin* w )
{
    QString qml = QStringLiteral( "qrc:/default.qml" );

    vlc_open();
    init_player_options();

    vlc_player_options& opts = get_options();
    const std::string& qml_source = opts.get_qml_source();
    if( !qml_source.empty() )
        qml = QString::fromUtf8( qml_source.data(), qml_source.size() );

    m_quickViewPtr.reset( new QQuickView );
    m_quickViewPtr->setResizeMode( QQuickView::SizeRootObjectToView );
    m_quickViewPtr->setProperty( "_q_embedded_native_parent_handle", WId( w->getHWND() ) );
    m_quickViewPtr->setFlags( m_quickViewPtr->flags()| Qt::FramelessWindowHint );

    QQmlContext* context = m_quickViewPtr->rootContext();
    m_qmlVlcPlayer = new QmlVlcPlayer( (vlc::player*)this, m_quickViewPtr.data() );
    context->setContextProperty( "vlcPlayer", QVariant::fromValue( m_qmlVlcPlayer ) );

    process_startup_options();

    m_quickViewPtr->setSource( qml );

    MoveWindow( (HWND)m_quickViewPtr->winId(), 0, 0, w->getWindowWidth(), w->getWindowHeight(), FALSE );
    m_quickViewPtr->show();

    return false;
}

bool Chimera_Win::onWindowDetached( FB::DetachedEvent *evt, FB::PluginWindowWin* w )
{
    return false;
}

bool Chimera_Win::onWindowResized( FB::ResizedEvent *evt, FB::PluginWindowWin* w )
{
    if( m_quickViewPtr ) {
        MoveWindow( (HWND)m_quickViewPtr->winId(), 0, 0, w->getWindowWidth(), w->getWindowHeight(), TRUE );
    }

    return false;
}

bool Chimera_Win::is_fullscreen()
{
    return false;
}

void Chimera_Win::set_fullscreen( bool fs )
{
}

void Chimera_Win::toggle_fullscreen()
{
}
