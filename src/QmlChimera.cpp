#include "QmlChimera.h"

#include <QGuiApplication>

#include <QmlVlc/QmlVlc.h>

#include "DOM/Window.h"

#include "JSRootQmlApi.h"

#if defined( FB_WIN ) && !defined( QT_NO_DEBUG )
#include <Shellapi.h>
#endif

void QmlChimera::StaticInitialize()
{
    RegisterQmlVlc();

    // Place one-time initialization stuff here; As of FireBreath 1.4 this should only
    // be called once per process
    if( !qApp ) {
#if defined( QT_NO_DEBUG ) || !defined( FB_WIN )
        static int argc = 0;
        new QGuiApplication( argc, 0 );
#else
        //Q_ASSERT(allArguments.size() == origArgc); at qcoreapplication.cpp:2109 workaround
        int argc;
        LocalFree( CommandLineToArgvW( GetCommandLineW(), &argc ) );
        std::vector<char*> argv( argc, nullptr );
        new QGuiApplication( argc, argv.data() );
#endif
        QGuiApplication::processEvents();
    }
}

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

void QmlChimera::load_startup_options()
{
   typedef boost::optional<std::string> param_type;
    typedef const FB::variant&          param_vtype;

    Chimera::load_startup_options();

    vlc_player_options& opts = get_options();

    param_type qml_source = getParam( "qmlsrc" );
    if ( qml_source )
        opts.set_qml_source( *qml_source );

    param_type qml = getParam( "qml" );
    if ( qml )
        opts.set_qml( *qml );
}

void QmlChimera::on_option_change( vlc_player_option_e o )
{
    Chimera::on_option_change( o );

    if( po_bg_color == o ) {
        Q_EMIT bgcolorChanged( get_bgColor() );
    }
}

std::string QmlChimera::getQmlError()
{
    return m_qmlError;
}

QUrl QmlChimera::getQmlSource()
{
    QUrl qml = QStringLiteral( "qrc:/default.qml" );

    std::string url = m_host->getDOMWindow()->getLocation();
    QUrl baseUrl = QString::fromUtf8( url.data(), url.size() );

    vlc_player_options& opts = get_options();
    const std::string& qml_source = opts.get_qml_source();
    if( !qml_source.empty() ) {
        QUrl qmlTmp = QString::fromUtf8( qml_source.data(), qml_source.size() );
        if( qmlTmp.isRelative() ) {
            qmlTmp = baseUrl.resolved( qmlTmp );
        }
#ifdef NDEBUG
        if( !qmlTmp.isLocalFile() ) {
            qml = qmlTmp;
        }
#else
        qml = qmlTmp;
#endif
    }

    return qml;
}

bool QmlChimera::isOptionTrusted( const std::string& option )
{
    return QmlVlcConfig::isOptionTrusted( QString::fromStdString( option ) );
}
