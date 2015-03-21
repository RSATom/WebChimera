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

#include "Chimera.h"

#include "JSRootQmlAPI.h"

#include <QmlVlc/QmlVlcConfig.h>

///////////////////////////////////////////////////////////////////////////////
/// @brief  Chimera constructor.  Note that your API is not available
///         at this point, nor the window.  For best results wait to use
///         the JSAPI object until the onPluginReady method is called
///////////////////////////////////////////////////////////////////////////////
Chimera::Chimera()
    : m_libvlc( 0 ), m_forceMute( false )
{
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Chimera destructor.
///////////////////////////////////////////////////////////////////////////////
Chimera::~Chimera()
{
    onMediaPlayerNotPlaying();

    // This is optional, but if you reset m_api (the shared_ptr to your JSAPI
    // root object) and tell the host to free the retained JSAPI objects then
    // unless you are holding another shared_ptr reference to your JSAPI object
    // they will be released here.
    releaseRootJSAPI();
    m_host->freeRetainedObjects();
}

//libvlc events arrives from separate thread
void Chimera::media_player_event( const libvlc_event_t* e )
{
    JSRootAPIPtr api  = boost::static_pointer_cast<JSRootAPI>( getRootJSAPI() );
    FB::BrowserHostPtr h = m_host;

    void (JSRootAPI::*event_to_fire)(void) = 0;
    bool scheduleNotPlaying = false;

    switch ( e->type ) {
    case libvlc_MediaPlayerMediaChanged:
        event_to_fire = &JSRootAPI::fire_MediaPlayerMediaChanged;
        break;
    case libvlc_MediaPlayerNothingSpecial:
        event_to_fire = &JSRootAPI::fire_MediaPlayerNothingSpecial;
        break;
    case libvlc_MediaPlayerOpening:
        event_to_fire = &JSRootAPI::fire_MediaPlayerOpening;
        break;
    case libvlc_MediaPlayerBuffering:
        h->ScheduleOnMainThread( api,
                                 boost::bind( &JSRootAPI::fire_MediaPlayerBuffering,
                                              api.get(),
                                              e->u.media_player_buffering.new_cache ) );
        break;
    case libvlc_MediaPlayerPlaying: {
        boost::shared_ptr<Chimera> thisPtr = FB::ptr_cast<Chimera>( shared_from_this() );
        h->ScheduleOnMainThread( thisPtr,
                                 boost::bind( &Chimera::onMediaPlayerPlaying,
                                              thisPtr ) );

        event_to_fire = &JSRootAPI::fire_MediaPlayerPlaying;
        break;
    }
    case libvlc_MediaPlayerPaused:
        event_to_fire = &JSRootAPI::fire_MediaPlayerPaused;
        scheduleNotPlaying = true;
        break;
    case libvlc_MediaPlayerStopped:
        event_to_fire = &JSRootAPI::fire_MediaPlayerStopped;
        scheduleNotPlaying = true;
        break;
    case libvlc_MediaPlayerForward:
        event_to_fire = &JSRootAPI::fire_MediaPlayerForward;
        break;
    case libvlc_MediaPlayerBackward:
        event_to_fire = &JSRootAPI::fire_MediaPlayerBackward;
        break;
    case libvlc_MediaPlayerEndReached:
        event_to_fire = &JSRootAPI::fire_MediaPlayerEndReached;
        scheduleNotPlaying = true;
        break;
    case libvlc_MediaPlayerEncounteredError:
        event_to_fire = &JSRootAPI::fire_MediaPlayerEncounteredError;
        scheduleNotPlaying = true;
        break;
    case libvlc_MediaPlayerTimeChanged: {
        double new_time = static_cast<double>( e->u.media_player_time_changed.new_time );
        h->ScheduleOnMainThread( api,
                                 boost::bind( &JSRootAPI::fire_MediaPlayerTimeChanged,
                                              api.get(),
                                              new_time ) );
        break;
    }
    case libvlc_MediaPlayerPositionChanged:
        h->ScheduleOnMainThread( api,
                                 boost::bind( &JSRootAPI::fire_MediaPlayerPositionChanged,
                                              api.get(),
                                              e->u.media_player_position_changed.new_position ) );
        break;
    case libvlc_MediaPlayerSeekableChanged:
        h->ScheduleOnMainThread( api,
                                 boost::bind( &JSRootAPI::fire_MediaPlayerSeekableChanged,
                                              api.get(),
                                              e->u.media_player_seekable_changed.new_seekable != 0 ) );
        break;
    case libvlc_MediaPlayerPausableChanged:
        h->ScheduleOnMainThread( api,
                                 boost::bind( &JSRootAPI::fire_MediaPlayerPausableChanged,
                                              api.get(),
                                              e->u.media_player_pausable_changed.new_pausable != 0 ) );
        break;
    //case libvlc_MediaPlayerTitleChanged:
    //    event_to_fire = &JSRootAPI::fire_MediaPlayerTitleChanged;
    //    break;
    //case libvlc_MediaPlayerSnapshotTaken:
    //    event_to_fire = &JSRootAPI::fire_MediaPlayerSnapshotTaken;
    //    break;
    case libvlc_MediaPlayerLengthChanged: {
       double new_length =
           static_cast<double>( e->u.media_player_length_changed.new_length );
        h->ScheduleOnMainThread( api,
                                 boost::bind( &JSRootAPI::fire_MediaPlayerLengthChanged,
                                              api.get(),
                                              new_length ) );

        break;
    }
    //case libvlc_MediaPlayerVout:
    //    event_to_fire = &JSRootAPI::fire_MediaPlayerVout;
    //    break;
    };

    if( event_to_fire ) {
        h->ScheduleOnMainThread( api, boost::bind( event_to_fire, api.get() ) );
    }

    if( scheduleNotPlaying ) {
        boost::shared_ptr<Chimera> thisPtr = FB::ptr_cast<Chimera>( shared_from_this() );
        h->ScheduleOnMainThread( thisPtr,
                                 boost::bind( &Chimera::onMediaPlayerNotPlaying,
                                              thisPtr ) );
    }
}
const FB::variant& Chimera::getParamVariant( const std::string& key ) const
{
    FB::VariantMap::const_iterator fnd = m_params.find( key.c_str() );
    if ( fnd != m_params.end() )
        return fnd->second;

    static const FB::variant empty;
    return empty;
}

void Chimera::loadStartupOptions()
{
    typedef boost::optional<std::string> param_type;
    typedef const FB::variant&           param_vtype;

    vlc_player_options& opts = get_options();

    param_vtype autoplay  = getParamVariant( "autoplay" );
    param_vtype autostart = getParamVariant( "autostart" );

    if ( !autoplay.empty() && autoplay.can_be_type<bool>() )
        opts.set_autoplay( autoplay.convert_cast<bool>() );
    if ( !autostart.empty() && autostart.can_be_type<bool>() )
        opts.set_autoplay( autostart.convert_cast<bool>() );

    param_vtype fs         = getParamVariant( "fullscreen" );
    param_vtype allowfs    = getParamVariant( "allowfullscreen" );
    param_vtype allowfs2   = getParamVariant( "allow-fullscreen" );
    param_vtype fsenabled  = getParamVariant( "fullscreenenabled" );
    param_vtype fsenabled2 = getParamVariant( "fullscreen-enabled" );
    if ( !fs.empty() && fs.can_be_type<bool>() )
        opts.set_enable_fs( fs.convert_cast<bool>() );
    if ( !allowfs.empty() && allowfs.can_be_type<bool>() )
        opts.set_enable_fs( allowfs.convert_cast<bool>() );
    if ( !allowfs2.empty() && allowfs2.can_be_type<bool>() )
        opts.set_enable_fs( allowfs2.convert_cast<bool>() );
    if ( !fsenabled.empty() && fsenabled.can_be_type<bool>() )
        opts.set_enable_fs( fsenabled.convert_cast<bool>() );
    if ( !fsenabled2.empty() && fsenabled2.can_be_type<bool>() )
        opts.set_enable_fs( fsenabled2.convert_cast<bool>() );

    param_type bgcolor = getParam( "bgcolor" );
    if ( bgcolor )
        opts.set_bg_color( *bgcolor );

    param_vtype use_proxy = getParamVariant( "use-proxy" );
    if ( !use_proxy.empty() && use_proxy.can_be_type<bool>() )
        opts.set_use_proxy( use_proxy.convert_cast<bool>() );
}

void Chimera::loadLibvlcOptions()
{
    typedef boost::optional<std::string> param_type;
    typedef const FB::variant&           param_vtype;

    QmlVlcConfig& config = QmlVlcConfig::instance();

    param_vtype network_caching = getParamVariant( "network-caching" );
    if( !network_caching.empty() && network_caching.can_be_type<int>() ) {
        config.setNetworkCacheTime( network_caching.convert_cast<int>( ) );
    };

    param_vtype adjust = getParamVariant( "adjust-filter" );
    if( !adjust.empty() && adjust.can_be_type<bool>() && adjust.convert_cast<bool>( ) ) {
        config.enableAdjustFilter( true );
    }

    param_vtype marq = getParamVariant( "marquee-filter" );
    if( !marq.empty() && marq.can_be_type<bool>() && marq.convert_cast<bool>( ) ) {
        config.enableMarqueeFilter( true );
    }

    param_vtype logo = getParamVariant( "logo-filter" );
    if( !logo.empty() && logo.can_be_type<bool>() && logo.convert_cast<bool>( ) ) {
        config.enableLogoFilter( true );
    }

    param_vtype debug = getParamVariant( "debug" );
    if( !debug.empty() && debug.can_be_type<bool>() && debug.convert_cast<bool>( ) ) {
        config.enableDebug( true );
    }

    param_vtype hw_accel = getParamVariant( "hw-accel" );
    if( !hw_accel.empty() && hw_accel.can_be_type<bool>() && hw_accel.convert_cast<bool>( ) ) {
        config.enableHardwareAcceleration( true );
    }

    config.enableNoVideoTitleShow( true );
}

void Chimera::vlcOpen()
{
    if( get_player().is_open() )
        return ;

    if( !m_libvlc ) {
        loadStartupOptions();
        loadLibvlcOptions();
        m_libvlc = QmlVlcConfig::instance().createLibvlcInstance();
    }

    if( m_libvlc && !get_player().is_open() ) {
        get_player().open( m_libvlc );
        get_player().register_callback( this );
     }
}

void Chimera::applyPlayerOptions()
{
    typedef boost::optional<std::string> param_type;
    typedef const FB::variant&           param_vtype;

    const vlc_player_options& opts = get_options();

    param_vtype mute            = getParamVariant( "mute" );
    if( !mute.empty() && mute.can_be_type<bool>() ) {
        m_forceMute = mute.convert_cast<bool>( );
        get_player().audio().set_mute( mute.convert_cast<bool>() );
    }

    param_vtype loop            = getParamVariant( "loop" );
    param_vtype autoloop        = getParamVariant( "autoloop" );
    bool set_loop = false;
    if ( !loop.empty() && loop.can_be_type<bool>() )
        set_loop = loop.convert_cast<bool>();
    if ( !autoloop.empty() && autoloop.can_be_type<bool>() )
        set_loop = autoloop.convert_cast<bool>();
    get_player().set_playback_mode( set_loop ?
                                    vlc::mode_loop :
                                    vlc::mode_normal );

    param_type target           = getParam( "target" );
    param_type mrl              = getParam( "mrl" );
    param_type filename         = getParam( "filename" );
    param_type src              = getParam( "src" );
    std::string set_mrl;
    if ( target )
        set_mrl = *target;
    if ( mrl )
        set_mrl = *mrl;
    if ( filename )
        set_mrl = *filename;
    if ( src )
        set_mrl = *src;
    if( !set_mrl.empty() ) {
        int item = addPlaylistItem( set_mrl.c_str() );
        if ( opts.get_autoplay() )
            get_player().play( item );
    }
}

void Chimera::vlcClose()
{
    get_player().stop();

    if ( get_player().is_open() ) {
        get_player().unregister_callback( this );
        get_player().close();
    }

    if ( m_libvlc ) {
        QmlVlcConfig::instance().releaseLibvlcInstance( m_libvlc );
        m_libvlc = 0;
    }
}

std::string Chimera::detectHttpProxy( const std::string& mrl ) const
{
    //detect proxy from browser
    typedef std::map<std::string, std::string> proxyInfo_t;
    typedef proxyInfo_t::const_iterator proxyInfo_it;
    proxyInfo_t proxyInfo;

    std::string proxy_str;
    if( get_options().get_use_proxy() && m_host->DetectProxySettings( proxyInfo, mrl ) ) {
        proxyInfo_it typeIt     = proxyInfo.find( "type" );
        proxyInfo_it hostNameIt = proxyInfo.find( "hostname" );
        proxyInfo_it portIt     = proxyInfo.find( "port" );

        if( proxyInfo.end() != typeIt && "http" == typeIt->second &&
            proxyInfo.end() != hostNameIt && !hostNameIt->second.empty() &&
            proxyInfo.end() != portIt && !portIt->second.empty() )
        {
            proxy_str = ":http-proxy=" + hostNameIt->second + ":" + portIt->second;
        }
    }

    return proxy_str;
}

int Chimera::addPlaylistItem( const std::string& mrl )
{
    return addPlaylistItem( mrl, std::vector<std::string>() );
}

int Chimera::addPlaylistItem( const std::string& mrl, const std::vector<std::string>& options )
{
    const std::string proxy_str = detectHttpProxy( mrl );

    //trusted options
    std::vector<const char*> trusted_opts;
    if( !proxy_str.empty() ) {
        trusted_opts.push_back( proxy_str.c_str() );
    }

    //untrusted options
    std::vector<const char*> untrusted_opts;
    for( unsigned i = 0; i < options.size(); ++i ) {
        if( isOptionTrusted( options[i] ) )
            trusted_opts.push_back( options[i].c_str() );
        else
            untrusted_opts.push_back( options[i].c_str() );
    }

    /*another trusted and untrusted options could be added here*/

    const char** untrusted_optv = untrusted_opts.empty() ? 0 : &untrusted_opts[0];
    const char** trusted_optv = trusted_opts.empty() ? 0 : &trusted_opts[0];
    return get_player().add_media( mrl.c_str(),
                                   untrusted_opts.size(), untrusted_optv,
                                   trusted_opts.size(), trusted_optv );
}


void Chimera::onPluginReady()
{
    // When this is called, the BrowserHost is attached, the JSAPI object is
    // created, and we are ready to interact with the page and such.  The
    // PluginWindow may or may not have already fire the AttachedEvent at
    // this point.
    vlcOpen();
}

void Chimera::shutdown()
{
    // This will be called when it is time for the plugin to shut down;
    // any threads or anything else that may hold a shared_ptr to this
    // object should be released here so that this object can be safely
    // destroyed. This is the last point that shared_from_this and weak_ptr
    // references to this object will be valid
    vlcClose();
}

void Chimera::on_option_change( vlc_player_option_e o )
{
}

void Chimera::onMediaPlayerPlaying()
{
    if( m_forceMute ) {
        //vlc 2.1.x "not mute on startup" bug workaround.
        get_player().audio().set_mute( true );
        m_forceMute = false;
    }
}

void Chimera::onMediaPlayerNotPlaying()
{
}
