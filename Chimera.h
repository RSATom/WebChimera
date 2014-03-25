/**********************************************************\

  Auto-generated Chimera.h

  This file contains the auto-generated main plugin object
  implementation for the Chimera Web Plugin project

\**********************************************************/
#ifndef H_CHIMERA_PLUGIN
#define H_CHIMERA_PLUGIN

#include "PluginWindow.h"
#include "PluginEvents/MouseEvents.h"
#include "PluginEvents/AttachedEvent.h"
#include "PluginEvents/DrawingEvents.h"

#include "PluginCore.h"

#include <vlc/vlc.h>

#include <QScopedPointer>
#include <QWeakPointer>
#include <QQuickView>

#include "libvlc_wrapper/vlc_player.h"
#include "vlc_player_options.h"

#include "QmlVlc/QmlVlcPlayer.h"

FB_FORWARD_PTR(Chimera)
class Chimera 
    : public FB::PluginCore,
      protected vlc_player,
      protected vlc_player_options
{
public:
    static void StaticInitialize();
    static void StaticDeinitialize();

public:
    Chimera();
    virtual ~Chimera();

public:
    void onPluginReady();
    void shutdown();
    virtual FB::JSAPIPtr createJSAPI();
    // If you want your plugin to always be windowless, set this to true
    // If you want your plugin to be optionally windowless based on the
    // value of the "windowless" param tag, remove this method or return
    // FB::PluginCore::isWindowless()
    virtual bool isWindowless() { return false; }

    BEGIN_PLUGIN_EVENT_MAP()
        EVENTTYPE_CASE( FB::DetachedEvent, onWindowDetached, FB::PluginWindow )
    END_PLUGIN_EVENT_MAP()

private:
    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    bool onWindowDetached( FB::DetachedEvent *evt, FB::PluginWindow* );
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

public:
    libvlc_instance_t* getLibVlc()
        { return m_libvlc; };
    vlc_player& get_player()
        { return *static_cast<vlc_player*>( this ); };
    vlc_player_options& get_options()
        { return *static_cast<vlc_player_options*>( this ); }
    const vlc_player_options& get_options() const
        { return *static_cast<const vlc_player_options*>( this ); }

    int add_playlist_item( const std::string& mrl );
    int add_playlist_item( const std::string& mrl, const std::vector<std::string>& options );

public:
    virtual bool is_fullscreen() = 0;
    virtual void set_fullscreen( bool fs ) = 0;
    virtual void toggle_fullscreen() = 0;
    std::string getQmlError();

private:
    const FB::variant& getParamVariant( const std::string& key ) const;

    void init_libvlc_options( std::vector<std::string>* );

    std::string detectHttpProxy( const std::string& mrl ) const;
    bool isTrustedOption( const std::string& option );

protected:
    void vlc_open();
    void process_startup_options();
    void vlc_close();

    void setBgColorQmlProperty();
    void setQml();

    QUrl getQmlSource();

protected:
    virtual void on_option_change( vlc_player_option_e );

private:
    void init_player_options();

    static void OnLibVlcEvent_proxy( const libvlc_event_t* e, void *param );
    void OnLibVlcEvent( const libvlc_event_t* e );
    void VlcEvents( bool Attach );

    void cleanQuickView();

protected:
    QScopedPointer<QQuickView> m_quickViewPtr;
    QmlVlcSurfacePlayerProxy* m_qmlVlcPlayer;

private:
    libvlc_instance_t* m_libvlc;
    std::string m_qmlError;
};

#endif

