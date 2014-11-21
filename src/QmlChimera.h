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
    static void StaticDeinitialize();

public:
    QmlChimera();

    QmlVlcSurfacePlayerProxy* getQmlVlcPlayer() const
        { return m_qmlVlcPlayer; }

    void setQml();
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
        EVENTTYPE_CASE( FB::DetachedEvent, onWindowDetached, FB::PluginWindow )
        PLUGIN_EVENT_MAP_CASCADE( Chimera )
    END_PLUGIN_EVENT_MAP()

private:
    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    bool onWindowDetached( FB::DetachedEvent *evt, FB::PluginWindow* );
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

protected:
    FB::JSAPIPtr createJSAPI() override;

    libvlc_instance_t* createLibvlcInstance() override;
    void load_startup_options() override;
    void load_libvlc_options();
    void apply_player_options() override;

    bool isOptionTrusted( const std::string& option ) override;

    void on_option_change( vlc_player_option_e o ) override;

private:
    QUrl getQmlSource();
    void cleanQuickView();

protected:
    QmlVlcSurfacePlayerProxy* m_qmlVlcPlayer;
    QScopedPointer<QQuickView> m_quickViewPtr;
    std::string m_qmlError;
};
