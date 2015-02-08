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

#include "QmlChimera.h"

#include <boost/algorithm/string.hpp>

#include <QGuiApplication>
#include <QDir>

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
        static int argc;
        LocalFree( CommandLineToArgvW( GetCommandLineW(), &argc ) );
        static char* argvStub = " ";
        static std::vector<char*> argv( argc, argvStub );
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

void QmlChimera::fireQmlStringMessage( int type, const QString& arg )
{
    JSRootQmlAPIPtr api  = boost::static_pointer_cast<JSRootQmlAPI>( getRootJSAPI() );
    api->fire_QmlStringMessage( type, arg.toStdString() );
}

void QmlChimera::fireQmlNumberMessage( int type, int arg1, int arg2 )
{
    JSRootQmlAPIPtr api  = boost::static_pointer_cast<JSRootQmlAPI>( getRootJSAPI() );
    api->fire_QmlNumberMessage( type, arg1, arg2 );
}

void QmlChimera::loadStartupOptions()
{
   typedef boost::optional<std::string> param_type;
    typedef const FB::variant&          param_vtype;

    Chimera::loadStartupOptions();

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

    if( po_bg_color == o )
        Q_EMIT bgcolorChanged( get_bgColor() );
}

std::string QmlChimera::getQmlError()
{
    return m_qmlError;
}

QUrl QmlChimera::getQmlSource()
{
    QUrl qml = QStringLiteral( "qrc:/default.qml" );

    FB::DOM::WindowPtr domWindow = m_host->getDOMWindow();
    std::string url = domWindow->getLocation();
    const bool isApp = boost::istarts_with( url, "app://" );
    QUrl baseUrl = QString::fromStdString( url );

    if( isApp && domWindow->getJSObject()->HasProperty( "process" ) ) {
        FB::JSObjectPtr process =
            domWindow->getProperty<FB::JSObjectPtr>( "process" );

        FB::variant vCwd = process->Invoke( "cwd", FB::variant_list_of() );
        if( vCwd.is_of_type<std::string>() ) {
            QString cwd =
                QDir::fromNativeSeparators(
                    QString::fromStdString( vCwd.cast<std::string>() ) );

            if( !cwd.endsWith( QDir::separator() ) )
                cwd += QDir::separator();

            baseUrl = QUrl::fromLocalFile( cwd );
        }
    }

    vlc_player_options& opts = get_options();
    const std::string& qml_source = opts.get_qml_source();
    if( !qml_source.empty() ) {
        QUrl qmlTmp = QString::fromUtf8( qml_source.data(), qml_source.size() );
        if( qmlTmp.isRelative() ) {
            qmlTmp = baseUrl.resolved( qmlTmp );
        }
#ifdef NDEBUG
        if( !qmlTmp.isLocalFile() || isApp ) {
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
