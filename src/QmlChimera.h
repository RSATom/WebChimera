#pragma once

#include <QScopedPointer>
#include <QQuickView>

#include <QmlVlc/QmlVlcPlayer.h>

#include "Chimera.h"

FB_FORWARD_PTR( QmlChimera )
class QmlChimera
    : public QObject, public Chimera
{
    Q_OBJECT

public:
    static void StaticInitialize();

public:
    QmlChimera();

    QmlVlcSurfacePlayerProxy* getQmlVlcPlayer() const
        { return m_qmlVlcPlayer; }

    std::string getQmlError();

    Q_PROPERTY( QString bgcolor READ get_bgColor NOTIFY bgcolorChanged )
    Q_PROPERTY( QmlVlcSurfacePlayerProxy* vlcPlayer READ getQmlVlcPlayer CONSTANT )
    Q_PROPERTY( bool fullscreen READ is_fullscreen WRITE set_fullscreen NOTIFY fullscreenChanged )

    QString get_bgColor() const;

    Q_INVOKABLE virtual void toggleFullscreen()
        { toggle_fullscreen(); };
    Q_INVOKABLE virtual void fireQmlMessage( const QString& message );

    void emitJsMessage( const std::string& message )
        { Q_EMIT jsMessage( QString::fromStdString( message ) ); }

Q_SIGNALS:
    void bgcolorChanged( const QString& bgcolor );
    void fullscreenChanged( bool fullscreen );
    void jsMessage( const QString& message );

public:
    BEGIN_PLUGIN_EVENT_MAP()
        PLUGIN_EVENT_MAP_CASCADE( Chimera )
    END_PLUGIN_EVENT_MAP()

private:
    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

protected:
    FB::JSAPIPtr createJSAPI() override;

    void load_startup_options() override;

    bool isOptionTrusted( const std::string& option ) override;

    void on_option_change( vlc_player_option_e o ) override;

    QUrl getQmlSource();

protected:
    QmlVlcSurfacePlayerProxy* m_qmlVlcPlayer;
    std::string m_qmlError;
};
