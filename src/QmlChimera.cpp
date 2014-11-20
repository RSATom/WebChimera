#include "QmlChimera.h"

#include <QGuiApplication>
#include <QQmlComponent>
#include <QQmlContext>

#include <QmlVlc/QmlVlc.h>

#include "DOM/Window.h"

#include "JSRootQmlApi.h"

void QmlChimera::StaticInitialize()
{
    Chimera::StaticInitialize();

    RegisterQmlVlc();

    // Place one-time initialization stuff here; As of FireBreath 1.4 this should only
    // be called once per process
    if( !qApp ) {
#ifdef QT_NO_DEBUG
        static int argc = 0;
        new QGuiApplication( argc, 0 );
#else
        //Q_ASSERT(allArguments.size() == origArgc); at qcoreapplication.cpp:2109 workaround
        static int argc = 1;
        static char** argv = { 0 };
        new QGuiApplication( argc, argv );
#endif
        QGuiApplication::processEvents();
    }
}

void QmlChimera::StaticDeinitialize()
{
    Chimera::StaticDeinitialize();
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

void QmlChimera::init_libvlc_options()
{
    typedef boost::optional<std::string> param_type;
    typedef const FB::variant&           param_vtype;

    param_vtype network_caching = getParamVariant( "network-caching" );
    if ( !network_caching.empty() && network_caching.can_be_type<int>() ) {
        QmlVlcConfig::setNetworkCacheTime( network_caching.convert_cast<int>() );
    };

    param_vtype adjust         = getParamVariant( "adjust-filter" );
    if ( !adjust.empty() && adjust.can_be_type<bool>() && adjust.convert_cast<bool>() ) {
        QmlVlcConfig::enableAdjustFilter( true );
    }

    param_vtype marq           = getParamVariant( "marquee-filter" );
    if ( !marq.empty() && marq.can_be_type<bool>() && marq.convert_cast<bool>() ) {
        QmlVlcConfig::enableMarqueeFilter( true );
    }

    param_vtype logo           = getParamVariant( "logo-filter" );
    if ( !logo.empty() && logo.can_be_type<bool>() && logo.convert_cast<bool>() ) {
        QmlVlcConfig::enableLogoFilter( true );
    }

    param_vtype debug          = getParamVariant( "debug" );
    if ( !debug.empty() && debug.can_be_type<bool>() && debug.convert_cast<bool>() ) {
        QmlVlcConfig::enableDebug( true );
    }

    param_vtype hw_accel         = getParamVariant( "hw-accel" );
    if ( !hw_accel.empty() && hw_accel.can_be_type<bool>() && hw_accel.convert_cast<bool>() ) {
        QmlVlcConfig::enableHardwareAcceleration( true );
    }

    QmlVlcConfig::enableNoVideoTitleShow( true );
}

libvlc_instance_t* QmlChimera::createLibvlcInstance()
{
    init_libvlc_options();
    return QmlVlcConfig::createLibvlcInstance();
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
    case po_qml:
        setQml();
        break;
    case po_bg_color:
        Q_EMIT bgcolorChanged( get_bgColor() );
        break;
    }
}

std::string QmlChimera::getQmlError()
{
    return m_qmlError;
}

void QmlChimera::setQml()
{
    if( !m_quickViewPtr )
        return;

    const std::string& qml = get_options().get_qml();
    QList<QQmlError> errors;
    if( qml.empty() ) {
        m_quickViewPtr->setSource( getQmlSource() );
        if( QQuickView::Error == m_quickViewPtr->status() )
            errors = m_quickViewPtr->errors();
    } else {
        QUrl qmlUrl = QStringLiteral( "qml" );
        QScopedPointer<QQmlComponent> component( new QQmlComponent( m_quickViewPtr->engine(), m_quickViewPtr.data() ) );

        component->setData( QByteArray( qml.data(), qml.size() ), qmlUrl );
        QObject* rootObject = component->create();
        if( QQmlComponent::Error == component->status() )
            errors = component->errors();

        if( rootObject ) {
            cleanQuickView();
            m_quickViewPtr->setContent( qmlUrl, component.take(), rootObject );
        }
    }

    if( !errors.empty() ) {
        QString errStr;
        for( int i = 0; i < errors.count(); ++i )
            errStr += errors[i].toString();
        m_qmlError = errStr.toStdString();
    } else
        m_qmlError.clear();
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

void QmlChimera::cleanQuickView()
{
    if( !m_quickViewPtr )
        return;

    //little hack to cleanup QQuickView content on Qt 5.2.
    //FIXME! check for compatibility with future Qt versions.
    m_quickViewPtr->setSource( QUrl() );
}

bool QmlChimera::isOptionTrusted( const std::string& option )
{
    return QmlVlcConfig::isOptionTrusted( QString::fromStdString( option ) );
}

bool QmlChimera::onWindowDetached( FB::DetachedEvent *evt, FB::PluginWindow* )
{
    m_quickViewPtr.reset();

    return false;
}
