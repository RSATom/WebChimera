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

#pragma once

#include <QUrl>
#include <QQuickItemGrabResult>

#include "Chimera.h"
#include "ChimeraPlayerProxy.h"

#if ( QT_VERSION >= QT_VERSION_CHECK( 5, 4, 0 ) )
#define SNAPSHOT_ENABLED 1
#endif

FB_FORWARD_PTR( QmlChimera )
class QmlChimera
    : public QObject, public Chimera
{
    Q_OBJECT

public:
    static void StaticInitialize();

public:
    QmlChimera();

    enum Platform {
        Windows = 0,
        X11 = 1,
        OSX = 2,
    };
    Q_ENUMS( Platform )

    QmlVlcPlayerProxy* getQmlVlcPlayer() const
        { return m_qmlVlcPlayer; }

    std::string getQmlError();

    Q_PROPERTY( QString version READ get_version CONSTANT )
    Q_PROPERTY( Platform platform READ get_platform CONSTANT )
    Q_PROPERTY( QString bgcolor READ get_bgColor NOTIFY bgcolorChanged )
    Q_PROPERTY( QmlVlcPlayerProxy* vlcPlayer READ getQmlVlcPlayer CONSTANT )
    Q_PROPERTY( bool fullscreen READ isFullscreen WRITE setFullscreen NOTIFY fullscreenChanged )

    Platform get_platform() const;
    QString get_version() const;
    QString get_bgColor() const;

    Q_INVOKABLE void toggleFullscreen() override
        { Chimera::toggleFullscreen(); }

    Q_INVOKABLE void fireQmlMessage( const QString& message );
    Q_INVOKABLE void fireQmlStringMessage( int type, const QString& arg );
    Q_INVOKABLE void fireQmlNumberMessage( int type, int arg1, int arg2 );

    Q_INVOKABLE void takeSnapshot( QQuickItem* );

    void emitJsMessage( const std::string& message )
        { Q_EMIT jsMessage( QString::fromStdString( message ) ); }

    Q_INVOKABLE QString toUtf8( const QByteArray& data, const QString& encoding );

    Q_INVOKABLE void goHome();

Q_SIGNALS:
    void bgcolorChanged( const QString& bgcolor );
    void fullscreenChanged( bool fullscreen );
    void jsMessage( const QString& message );
    void snapshotReady( const QString& snapshot );

private Q_SLOTS:
    void grabResultReady();

public:
    BEGIN_PLUGIN_EVENT_MAP()
        PLUGIN_EVENT_MAP_CASCADE( Chimera )
    END_PLUGIN_EVENT_MAP()

private:
    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

protected:
    FB::JSAPIPtr createJSAPI() override;

    void loadStartupOptions() override;

    bool isOptionTrusted( const std::string& option ) override;

    void on_option_change( vlc_player_option_e o ) override;

    QUrl getQmlSource();

protected:
    QPointer<ChimeraPlayerProxy> m_qmlVlcPlayer;

    std::string m_qmlError;
#ifdef SNAPSHOT_ENABLED
    QSharedPointer<QQuickItemGrabResult> m_itemGrabResult;
#endif
};
