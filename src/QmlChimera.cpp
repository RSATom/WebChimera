#include "QmlChimera.h"

#include "JSRootQmlApi.h"

QmlChimera::QmlChimera()
    : m_qmlVlcPlayer( 0 )
{

}
FB::JSAPIPtr QmlChimera::createJSAPI()
{
    return boost::make_shared<JSRootQmlAPI>( FB::ptr_cast<QmlChimera>( shared_from_this() ), m_host );
}

QString QmlChimera::get_bgColor() const
{
    return QString::fromStdString( get_bg_color() );
}

void QmlChimera::fireQmlMessage( const QString& message )
{
    JSRootQmlAPIPtr api  = boost::static_pointer_cast<JSRootQmlAPI>( getRootJSAPI() );
    api->fire_QmlMessage( message.toStdString() );
}

void QmlChimera::process_startup_options()
{
    assert( m_quickViewPtr );
    if( m_quickViewPtr ) {
        QQmlContext* context = m_quickViewPtr->rootContext();
        context->setContextObject( this );
        context->setContextProperty( QStringLiteral( "plugin" ), this );
    }

    Chimera::process_startup_options();
}

void QmlChimera::on_option_change( vlc_player_option_e o )
{
    Chimera::on_option_change( o );

    switch( o ) {
    case po_bg_color:
        Q_EMIT bgcolorChanged( get_bgColor() );
        break;
    }
}
