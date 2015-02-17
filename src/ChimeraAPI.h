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

#include <string>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "Chimera.h"

#include <vlc/vlc.h>

#ifndef H_CHIMERA_API
#define H_CHIMERA_API

////////////////////////////////////////////////////////////////////////////
/// JSAudioAPI
////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR(JSAudioAPI)
class JSAudioAPI : public FB::JSAPIAuto
{
public:
    JSAudioAPI( const ChimeraPtr& plugin, const FB::BrowserHostPtr& host )
        : m_plugin( plugin ), m_host( host )
    {
        registerAttribute( "Error",         libvlc_AudioChannel_Error,   true );
        registerAttribute( "Stereo",        libvlc_AudioChannel_Stereo,  true );
        registerAttribute( "ReverseStereo", libvlc_AudioChannel_RStereo, true );
        registerAttribute( "Left",          libvlc_AudioChannel_Left,    true );
        registerAttribute( "Right",         libvlc_AudioChannel_Right,   true );
        registerAttribute( "Dolby",         libvlc_AudioChannel_Dolbys,  true );

        registerProperty( "count",
                          make_property( this, &JSAudioAPI::get_trackCount ) );

        registerProperty( "track",
                          make_property( this, &JSAudioAPI::get_track,
                                               &JSAudioAPI::set_track ) );
        registerProperty( "mute",
                          make_property( this, &JSAudioAPI::get_mute,
                                               &JSAudioAPI::set_mute ) );
        registerProperty( "volume",
                          make_property( this, &JSAudioAPI::get_volume,
                                               &JSAudioAPI::set_volume ) );
        registerProperty( "channel",
                          make_property( this, &JSAudioAPI::get_channel,
                                               &JSAudioAPI::set_channel ) );
        registerProperty( "delay",
                          make_property( this, &JSAudioAPI::get_delay,
                                               &JSAudioAPI::set_delay ) );

        registerMethod( "toggleMute",
                        make_method( this, &JSAudioAPI::toggleMute ) );
        registerMethod( "description",
                        make_method( this, &JSAudioAPI::description ) );
    }

    virtual ~JSAudioAPI() {};

    ChimeraPtr getPlugin();

    unsigned get_trackCount();

    bool get_mute();
    void set_mute( bool );

    unsigned int get_volume();
    void set_volume( unsigned int );

    int get_track();
    void set_track( int idx );

    unsigned int get_channel();
    void set_channel( unsigned int );

    int get_delay();
    void set_delay( int );

    void toggleMute();

    std::string description( unsigned int trackID );

private:
    ChimeraWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;
};

////////////////////////////////////////////////////////////////////////////
/// JSInputAPI
////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR(JSInputAPI)
class JSInputAPI : public FB::JSAPIAuto
{
public:
    JSInputAPI( const ChimeraPtr& plugin, const FB::BrowserHostPtr& host )
        :m_plugin( plugin ), m_host( host )
    {
        registerProperty( "length",
                          make_property( this, &JSInputAPI::get_length ) );
        registerProperty( "fps",
                          make_property( this, &JSInputAPI::get_fps ) );
        registerProperty( "hasVout",
                          make_property( this, &JSInputAPI::get_hasVout ) );
        registerProperty( "state",
                          make_property( this, &JSInputAPI::get_state ) );

        registerProperty( "position",
                          make_property( this, &JSInputAPI::get_position,
                                               &JSInputAPI::set_position ) );
        registerProperty( "time",
                          make_property( this, &JSInputAPI::get_time,
                                               &JSInputAPI::set_time ) );
        registerProperty( "rate",
                          make_property( this, &JSInputAPI::get_rate,
                                               &JSInputAPI::set_rate ) );
    }

    virtual ~JSInputAPI(){}

    ChimeraPtr getPlugin();

    double get_length();
    double get_fps();
    bool get_hasVout();
    unsigned int get_state();

    double get_position();
    void set_position( double );

    double get_time();
    void set_time( double );

    double get_rate();
    void set_rate( double );

private:
    ChimeraWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;
};

////////////////////////////////////////////////////////////////////////////
/// JSPlaylistItemsAPI
////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR(JSPlaylistItemsAPI)
class JSPlaylistItemsAPI : public FB::JSAPIAuto
{
public:
    JSPlaylistItemsAPI( const ChimeraPtr& plugin, const FB::BrowserHostPtr& host )
        :m_plugin( plugin ), m_host( host )
    {
        registerProperty( "count",
                          make_property( this, &JSPlaylistItemsAPI::get_count ) );

        registerMethod( "clear",
                        make_method( this, &JSPlaylistItemsAPI::clear ) );
        registerMethod( "remove",
                        make_method( this, &JSPlaylistItemsAPI::remove ) );
    }

    virtual ~JSPlaylistItemsAPI(){}

    ChimeraPtr getPlugin();

    unsigned int get_count();
    void clear();
    bool remove( unsigned int idx );

    FB::variant GetProperty( int idx ) override;
    FB::variant GetProperty( const std::string& propertyName ) override;

private:
    ChimeraWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;
};

////////////////////////////////////////////////////////////////////////////
/// JSPlaylistAPI
////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR(JSPlaylistAPI)
class JSPlaylistAPI : public FB::JSAPIAuto
{
public:
    JSPlaylistAPI( const ChimeraPtr& plugin, const FB::BrowserHostPtr& host )
        :m_plugin( plugin ), m_host( host )
    {
        registerAttribute( "Normal", vlc::mode_normal, true );
        registerAttribute( "Loop",   vlc::mode_loop,   true );
        registerAttribute( "Single", vlc::mode_last,   true );


        registerProperty( "itemCount",
                          make_property( this, &JSPlaylistAPI::get_itemCount ) );
        registerProperty( "isPlaying",
                          make_property( this, &JSPlaylistAPI::get_isPlaying ) );

        registerProperty( "currentItem",
                          make_property( this, &JSPlaylistAPI::get_current,
                                               &JSPlaylistAPI::set_current ) );
        registerProperty( "mode",
                          make_property( this, &JSPlaylistAPI::get_mode,
                                               &JSPlaylistAPI::set_mode ) );

        registerMethod( "add",
                        make_method( this, &JSPlaylistAPI::add ) );
        registerMethod( "addWithOptions",
                        make_method( this, &JSPlaylistAPI::addWithOptions ) );
        registerMethod( "play",
                        make_method( this, &JSPlaylistAPI::play ) );
        registerMethod( "playItem",
                        make_method( this, &JSPlaylistAPI::playItem ) );
        registerMethod( "pause",
                        make_method( this, &JSPlaylistAPI::pause ) );
        registerMethod( "togglePause",
                        make_method( this, &JSPlaylistAPI::togglePause ) );
        registerMethod( "stop",
                        make_method( this, &JSPlaylistAPI::stop ) );
        registerMethod( "next",
                        make_method( this, &JSPlaylistAPI::next ) );
        registerMethod( "prev",
                        make_method( this, &JSPlaylistAPI::prev ) );
        registerMethod( "clear",
                        make_method( this, &JSPlaylistAPI::clear ) );
        registerMethod( "removeItem",
                       make_method( this, &JSPlaylistAPI::removeItem ) );
        registerMethod( "advanceItem",
                       make_method( this, &JSPlaylistAPI::advanceItem ) );

        m_items = boost::make_shared<JSPlaylistItemsAPI>( plugin, m_host );
        registerProperty( "items", make_property( this, &JSPlaylistAPI::get_items ) );
    }

    virtual ~JSPlaylistAPI(){}

    ChimeraPtr getPlugin();

    unsigned int get_itemCount();
    bool get_isPlaying();

    int get_current();
    void set_current( unsigned idx );

    unsigned get_mode();
    void set_mode( unsigned );

    int add( const std::string& mrl );
    int addWithOptions( const std::string& mrl,
                        const std::vector<std::string>& options );

    void play();
    bool playItem( unsigned idx );
    void pause();
    void togglePause();
    void stop();
    void next();
    void prev();
    void clear();
    bool removeItem( unsigned int idx );
    void advanceItem( unsigned idx, int count );

    JSPlaylistItemsAPIPtr get_items() { return m_items; }

private:
    ChimeraWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    JSPlaylistItemsAPIPtr m_items;
};

////////////////////////////////////////////////////////////////////////////
/// JSSubtitleAPI
////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR(JSSubtitleAPI)
class JSSubtitleAPI : public FB::JSAPIAuto
{
public:
    JSSubtitleAPI( const ChimeraPtr& plugin, const FB::BrowserHostPtr& host )
        :m_plugin( plugin ), m_host( host )
    {
        registerProperty( "count",
                          make_property( this, &JSSubtitleAPI::get_trackCount ) );

        registerProperty( "track",
                          make_property( this, &JSSubtitleAPI::get_track,
                                               &JSSubtitleAPI::set_track ) );
        registerProperty( "delay",
                          make_property( this, &JSSubtitleAPI::get_delay,
                                               &JSSubtitleAPI::set_delay ) );

        registerMethod( "description",
                        make_method( this, &JSSubtitleAPI::description ) );
    }

    virtual ~JSSubtitleAPI(){}

    ChimeraPtr getPlugin();

    unsigned get_trackCount();

    int get_track();
    void set_track( int idx );

    int get_delay();
    void set_delay( int );

    std::string description( unsigned int i );

private:
    ChimeraWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;
};

////////////////////////////////////////////////////////////////////////////
/// JSMarqueeAPI
////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR(JSMarqueeAPI)
class JSMarqueeAPI : public FB::JSAPIAuto
{
public:
    JSMarqueeAPI( const ChimeraPtr& plugin, const FB::BrowserHostPtr& host )
        :m_plugin( plugin ), m_host( host )
    {
        registerProperty( "text",
                          make_property( this, &JSMarqueeAPI::get_text,
                                               &JSMarqueeAPI::set_text ) );
        registerProperty( "color",
                          make_property( this, &JSMarqueeAPI::get_color,
                                               &JSMarqueeAPI::set_color ) );
        registerProperty( "opacity",
                          make_property( this, &JSMarqueeAPI::get_opacity,
                                               &JSMarqueeAPI::set_opacity ) );
        registerProperty( "position",
                          make_property( this, &JSMarqueeAPI::get_position,
                                               &JSMarqueeAPI::set_position ) );
        registerProperty( "refresh",
                          make_property( this, &JSMarqueeAPI::get_refresh,
                                               &JSMarqueeAPI::set_refresh ) );
        registerProperty( "size",
                          make_property( this, &JSMarqueeAPI::get_size,
                                               &JSMarqueeAPI::set_size ) );
        registerProperty( "timeout",
                          make_property( this, &JSMarqueeAPI::get_timeout,
                                               &JSMarqueeAPI::set_timeout ) );
        registerProperty( "x",
                          make_property( this, &JSMarqueeAPI::get_x,
                                               &JSMarqueeAPI::set_x ) );
        registerProperty( "y",
                          make_property( this, &JSMarqueeAPI::get_y,
                                               &JSMarqueeAPI::set_y ) );

        registerMethod( "enable",
                        make_method( this, &JSMarqueeAPI::enable ) );
        registerMethod( "disable",
                        make_method( this, &JSMarqueeAPI::disable ) );
    }

    virtual ~JSMarqueeAPI(){}

    ChimeraPtr getPlugin();

    std::string get_text();
    void set_text( const std::string& t );

    std::string get_position();
    void set_position( const std::string& p );

    unsigned int get_color()
        {  return get_marquee_int( libvlc_marquee_Color ); };
    void set_color( unsigned int c )
        {  set_marquee_int( libvlc_marquee_Color, c ); };

    unsigned int get_opacity()
        {  return get_marquee_int( libvlc_marquee_Opacity ); };
    void set_opacity( unsigned int o )
        {  set_marquee_int( libvlc_marquee_Opacity, o ); };

    unsigned int get_refresh()
        {  return get_marquee_int( libvlc_marquee_Refresh ); };
    void set_refresh( unsigned int r )
        {  set_marquee_int( libvlc_marquee_Refresh, r ); };

    unsigned int get_size()
        {  return get_marquee_int( libvlc_marquee_Size ); };
    void set_size( unsigned int s )
        {  set_marquee_int( libvlc_marquee_Size, s ); };

    unsigned int get_timeout()
        {  return get_marquee_int( libvlc_marquee_Timeout ); };
    void set_timeout( unsigned int t )
        {  set_marquee_int( libvlc_marquee_Timeout, t ); };

    unsigned int get_x()
        {  return get_marquee_int( libvlc_marquee_X ); };
    void set_x( unsigned int x )
        {  set_marquee_int( libvlc_marquee_X, x ); };

    unsigned int get_y()
        {  return get_marquee_int( libvlc_marquee_Y ); };
    void set_y( unsigned int y )
        {  set_marquee_int( libvlc_marquee_Y, y ); };

    void enable()
        { set_marquee_int( libvlc_marquee_Enable, 1 ); };
    void disable()
        { set_marquee_int( libvlc_marquee_Enable, 0 ); };

private:
    int get_marquee_int( libvlc_video_marquee_option_t );
    void set_marquee_int( libvlc_video_marquee_option_t, int i );

private:
    ChimeraWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;
};

////////////////////////////////////////////////////////////////////////////
/// JSLogoAPI
////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR(JSLogoAPI)
class JSLogoAPI : public FB::JSAPIAuto
{
public:
    JSLogoAPI( const ChimeraPtr& plugin, const FB::BrowserHostPtr& host )
        :m_plugin( plugin ), m_host( host )
    {
        registerProperty( "position",
                          make_property( this, &JSLogoAPI::get_position,
                                               &JSLogoAPI::set_position ) );
        registerProperty( "opacity",
                          make_property( this, &JSLogoAPI::get_opacity,
                                               &JSLogoAPI::set_opacity ) );
        registerProperty( "delay",
                          make_property( this, &JSLogoAPI::get_delay,
                                               &JSLogoAPI::set_delay ) );
        registerProperty( "repeat",
                          make_property( this, &JSLogoAPI::get_repeat,
                                               &JSLogoAPI::set_repeat ) );
        registerProperty( "x",
                          make_property( this, &JSLogoAPI::get_x,
                                               &JSLogoAPI::set_x ) );
        registerProperty( "y",
                          make_property( this, &JSLogoAPI::get_y,
                                             &JSLogoAPI::set_y ) );

        registerMethod( "enable",
                        make_method( this, &JSLogoAPI::enable ) );
        registerMethod( "disable",
                        make_method( this, &JSLogoAPI::disable ) );
        registerMethod( "file",
                        make_method( this, &JSLogoAPI::file ) );
    }

    virtual ~JSLogoAPI(){}

    ChimeraPtr getPlugin();

    std::string get_position();
    void set_position( const std::string& );

    unsigned int get_opacity()
        { return get_logo_int( libvlc_logo_opacity ); };
    void set_opacity( unsigned int o )
        { set_logo_int( libvlc_logo_opacity, o ); };

    unsigned int get_delay()
        { return get_logo_int( libvlc_logo_delay ); };
    void set_delay( unsigned int d )
        { set_logo_int( libvlc_logo_delay, d ); };

    int get_repeat()
        { return get_logo_int( libvlc_logo_repeat ); };
    void set_repeat( int r )
        { set_logo_int( libvlc_logo_repeat, r ); };

    unsigned int get_x()
        { return get_logo_int( libvlc_logo_x ); };
    void set_x( unsigned int x )
        { set_logo_int( libvlc_logo_x, x ); };

    unsigned int get_y()
        { return get_logo_int( libvlc_logo_y ); };
    void set_y( unsigned int y )
        { set_logo_int( libvlc_logo_y, y ); };

    void enable()
        { return set_logo_int( libvlc_logo_enable, 1 ); }
    void disable()
        { return set_logo_int( libvlc_logo_enable, 0 ); }

    void file( const std::string& );

private:
    int get_logo_int( libvlc_video_logo_option_t );
    void set_logo_int( libvlc_video_logo_option_t, int i );

private:
    ChimeraWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;
};

////////////////////////////////////////////////////////////////////////////
/// JSDeinterlaceAPI
////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR(JSDeinterlaceAPI)
class JSDeinterlaceAPI : public FB::JSAPIAuto
{
public:
    JSDeinterlaceAPI( const ChimeraPtr& plugin, const FB::BrowserHostPtr& host )
        :m_plugin( plugin ), m_host( host )
    {
        registerMethod( "enable",
                        make_method( this, &JSDeinterlaceAPI::enable ) );
        registerMethod( "disable",
                        make_method( this, &JSDeinterlaceAPI::disable ) );
    }

    virtual ~JSDeinterlaceAPI(){}

    ChimeraPtr getPlugin();

    void enable( const std::string& mode );
    void disable();

private:
    ChimeraWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;
};

////////////////////////////////////////////////////////////////////////////
/// JSVideoAPI
////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR(JSVideoAPI)
class JSVideoAPI : public FB::JSAPIAuto
{
public:
    JSVideoAPI( const ChimeraPtr& plugin, const FB::BrowserHostPtr& host )
        :m_plugin( plugin ), m_host( host )
    {
        registerProperty( "width",
                          make_property( this, &JSVideoAPI::get_width ) );
        registerProperty( "height",
                          make_property( this, &JSVideoAPI::get_height ) );
        registerProperty( "count",
                          make_property( this,
                                         &JSVideoAPI::get_trackCount ) );

        registerProperty( "track",
                          make_property( this, &JSVideoAPI::get_track,
                                               &JSVideoAPI::set_track ) );
        registerProperty( "aspectRatio",
                          make_property( this, &JSVideoAPI::get_aspectRatio,
                                               &JSVideoAPI::set_aspectRatio ) );
        registerProperty( "crop",
                          make_property( this, &JSVideoAPI::get_crop,
                                               &JSVideoAPI::set_crop ) );
        registerProperty( "contrast",
                          make_property( this, &JSVideoAPI::get_contrast,
                                               &JSVideoAPI::set_contrast ) );
        registerProperty( "brightness",
                          make_property( this, &JSVideoAPI::get_brightness,
                                               &JSVideoAPI::set_brightness ) );
        registerProperty( "hue",
                          make_property( this, &JSVideoAPI::get_hue,
                                               &JSVideoAPI::set_hue ) );
        registerProperty( "saturation",
                          make_property( this, &JSVideoAPI::get_saturation,
                                               &JSVideoAPI::set_saturation ) );
        registerProperty( "gamma",
                          make_property( this, &JSVideoAPI::get_gamma,
                                               &JSVideoAPI::set_gamma ) );
        registerProperty( "teletext",
                          make_property( this, &JSVideoAPI::get_teletext,
                                              &JSVideoAPI::set_teletext ) );
        registerProperty( "fullscreen",
                          make_property( this, &JSVideoAPI::get_fullscreen,
                                               &JSVideoAPI::set_fullscreen ) );


        registerMethod( "toggleTeletext",
                        make_method( this, &JSVideoAPI::toggleTeletext ) );
        registerMethod( "toggleFullscreen",
                        make_method( this, &JSVideoAPI::toggleFullscreen ) );

        m_marquee = boost::make_shared<JSMarqueeAPI>( plugin, m_host );
        registerProperty( "marquee", make_property( this, &JSVideoAPI::get_marquee ) );

        m_logo = boost::make_shared<JSLogoAPI>( plugin, m_host );
        registerProperty( "logo", make_property( this, &JSVideoAPI::get_logo ) );

        m_deinterlace = boost::make_shared<JSDeinterlaceAPI>( plugin, m_host );
        registerProperty( "deinterlace", make_property( this, &JSVideoAPI::get_deinterlace ) );
    }

    virtual ~JSVideoAPI(){}

    ChimeraPtr getPlugin();

    unsigned int get_width();
    unsigned int get_height();

    unsigned get_trackCount();

    int get_track();
    void set_track( int idx );

    bool get_fullscreen();
    void set_fullscreen( bool fs );

    void toggleFullscreen();

    std::string get_aspectRatio();
    void set_aspectRatio( const std::string& );

    std::string get_crop();
    void set_crop( const std::string& );

    int get_teletext();
    void set_teletext( unsigned int );

    void toggleTeletext();

    float get_contrast();
    void set_contrast( float v );

    float get_brightness();
    void set_brightness( float v );

    float get_hue();
    void set_hue( float v );

    float get_saturation();
    void set_saturation( float v );

    float get_gamma();
    void set_gamma( float v );

    JSMarqueeAPIWeakPtr     get_marquee()     {return m_marquee;};
    JSLogoAPIWeakPtr        get_logo()        {return m_logo;}
    JSDeinterlaceAPIWeakPtr get_deinterlace() {return m_deinterlace;}

private:
    void getVideoSize( unsigned* width, unsigned* height );

private:
    ChimeraWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    JSMarqueeAPIPtr     m_marquee;
    JSLogoAPIPtr        m_logo;
    JSDeinterlaceAPIPtr m_deinterlace;
};

////////////////////////////////////////////////////////////////////////////
/// JSMediaDescAPI
////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR(JSMediaDescAPI)
class JSMediaDescAPI : public FB::JSAPIAuto
{
public:
    JSMediaDescAPI( const ChimeraPtr& plugin )
        :m_plugin( plugin )
    {

        registerProperty( "artist",
                          make_property( this, &JSMediaDescAPI::get_artist ) );
        registerProperty( "genre",
                          make_property( this, &JSMediaDescAPI::get_genre ) );
        registerProperty( "copyright",
                          make_property( this, &JSMediaDescAPI::get_copyright ) );
        registerProperty( "album",
                          make_property( this, &JSMediaDescAPI::get_album ) );
        registerProperty( "trackNumber",
                          make_property( this, &JSMediaDescAPI::get_trackNumber ) );
        registerProperty( "description",
                          make_property( this, &JSMediaDescAPI::get_description ) );
        registerProperty( "rating",
                          make_property( this, &JSMediaDescAPI::get_rating ) );
        registerProperty( "date",
                          make_property( this, &JSMediaDescAPI::get_date ) );
        registerProperty( "URL",
                          make_property( this, &JSMediaDescAPI::get_URL ) );
        registerProperty( "language",
                          make_property( this, &JSMediaDescAPI::get_language ) );
        registerProperty( "nowPlaying",
                          make_property( this, &JSMediaDescAPI::get_nowPlaying ) );
        registerProperty( "publisher",
                          make_property( this, &JSMediaDescAPI::get_publisher ) );
        registerProperty( "encodedBy",
                          make_property( this, &JSMediaDescAPI::get_encodedBy ) );
        registerProperty( "artworkURL",
                          make_property( this, &JSMediaDescAPI::get_artworkURL ) );
        registerProperty( "trackID",
                         make_property( this, &JSMediaDescAPI::get_trackID ) );
        registerProperty( "mrl",
                          make_property( this, &JSMediaDescAPI::get_mrl ) );

        registerProperty( "title",
                          make_property( this, &JSMediaDescAPI::get_title,
                                               &JSMediaDescAPI::set_title ) );
        registerProperty( "setting",
                          make_property( this, &JSMediaDescAPI::get_setting,
                                               &JSMediaDescAPI::set_setting ) );

        registerProperty( "disabled",
                          make_property( this, &JSMediaDescAPI::get_disabled,
                                               &JSMediaDescAPI::set_disabled ) );
    }

    virtual ~JSMediaDescAPI(){}

    ChimeraPtr getPlugin();

    std::string get_title();
    void set_title( const std::string& );

    std::string get_artist();
    std::string get_genre();
    std::string get_copyright();
    std::string get_album();
    std::string get_trackNumber();
    std::string get_description();
    std::string get_rating();
    std::string get_date();

    std::string get_setting();
    void set_setting( const std::string& );

    std::string get_URL();
    std::string get_language();
    std::string get_nowPlaying();
    std::string get_publisher();
    std::string get_encodedBy();
    std::string get_artworkURL();
    std::string get_trackID();
    std::string get_mrl();

    bool get_disabled();
    void set_disabled( bool disabled );

private:
    std::string get_meta( libvlc_meta_t e_meta );
    void set_meta( libvlc_meta_t e_meta, const std::string& );

protected:
    virtual vlc::media get_media() = 0;

private:
    ChimeraWeakPtr m_plugin;
};

////////////////////////////////////////////////////////////////////////////
/// JSCurrentMediaDescAPI
////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR( JSCurrentMediaDescAPI )
class JSCurrentMediaDescAPI : public JSMediaDescAPI
{
public:
    JSCurrentMediaDescAPI( const ChimeraPtr& plugin )
        : JSMediaDescAPI( plugin ) {}

protected:
    virtual vlc::media get_media();
};

////////////////////////////////////////////////////////////////////////////
/// JSMediaMediaDescAPI
////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR( JSMediaMediaDescAPI )
class JSMediaMediaDescAPI : public JSMediaDescAPI
{
public:
    JSMediaMediaDescAPI( const ChimeraPtr& plugin, const vlc::media& );
    ~JSMediaMediaDescAPI();

protected:
    virtual vlc::media get_media();

private:
    vlc::media m_media;
};

////////////////////////////////////////////////////////////////////////////
/// JSRootAPI
////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR(JSRootAPI)
class JSRootAPI : public FB::JSAPIAuto
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /// @fn JSRootAPI::JSRootAPI(const ChimeraPtr& plugin, const FB::BrowserHostPtr host)
    ///
    /// @brief  Constructor for your JSAPI object.
    ///         You should register your methods, properties, and events
    ///         that should be accessible to Javascript from here.
    ///
    /// @see FB::JSAPIAuto::registerMethod
    /// @see FB::JSAPIAuto::registerProperty
    /// @see FB::JSAPIAuto::registerEvent
    ////////////////////////////////////////////////////////////////////////////
    JSRootAPI( const ChimeraPtr& plugin, const FB::BrowserHostPtr& host ) :
        m_plugin( plugin ), m_host( host )
    {
        // Read-only property
        registerProperty( "version",
                          make_property( this,
                                         &JSRootAPI::get_version ) );
        registerProperty( "vlcVersion",
                          make_property( this,
                                         &JSRootAPI::get_vlcVersion ) );
        registerProperty( "playing",
                          make_property( this,
                                         &JSRootAPI::get_playing ) );
        registerProperty( "length",
                          make_property( this,
                                         &JSRootAPI::get_length ) );
        registerProperty( "state",
                          make_property( this,
                                          &JSRootAPI::get_state ) );

        registerProperty( "position",
                          make_property( this, &JSRootAPI::get_position,
                                               &JSRootAPI::set_position ) );
        registerProperty( "time",
                          make_property( this, &JSRootAPI::get_time,
                                               &JSRootAPI::set_time ) );
        registerProperty( "volume",
                          make_property( this, &JSRootAPI::get_volume,
                                               &JSRootAPI::set_volume ) );
        registerProperty( "bgcolor",
                          make_property( this, &JSRootAPI::get_bgcolor,
                                               &JSRootAPI::set_bgcolor ) );
        registerProperty( "fullscreen",
                          make_property( this, &JSRootAPI::get_fullscreen,
                                               &JSRootAPI::set_fullscreen ) );

        registerMethod( "play",             make_method( this, &JSRootAPI::play ) );
        registerMethod( "pause",            make_method( this, &JSRootAPI::pause ) );
        registerMethod( "togglePause",      make_method( this, &JSRootAPI::togglePause ) );
        registerMethod( "stop",             make_method( this, &JSRootAPI::stop ) );
        registerMethod( "toggleMute",       make_method( this, &JSRootAPI::toggleMute ) );
        registerMethod( "toggleFullscreen", make_method( this, &JSRootAPI::toggleFullscreen ) );

        registerAttribute( "NothingSpecial", libvlc_NothingSpecial, true );
        registerAttribute( "Opening",        libvlc_Opening,        true );
        registerAttribute( "Buffering",      libvlc_Buffering,      true );
        registerAttribute( "Playing",        libvlc_Playing,        true );
        registerAttribute( "Paused",         libvlc_Paused,         true );
        registerAttribute( "Stopped",        libvlc_Stopped,        true );
        registerAttribute( "Ended",          libvlc_Ended,          true );
        registerAttribute( "Error",          libvlc_Error,          true );

        m_audio = boost::make_shared<JSAudioAPI>( plugin, m_host );
        registerProperty( "audio", make_property( this, &JSRootAPI::get_audio ) );

        m_input = boost::make_shared<JSInputAPI>( plugin, m_host );
        registerProperty( "input", make_property( this, &JSRootAPI::get_input ) );

        m_playlist = boost::make_shared<JSPlaylistAPI>( plugin, m_host );
        registerProperty( "playlist", make_property( this, &JSRootAPI::get_playlist ) );

        m_subtitle = boost::make_shared<JSSubtitleAPI>( plugin, m_host );
        registerProperty( "subtitle", make_property( this, &JSRootAPI::get_subtitle ) );

        m_video = boost::make_shared<JSVideoAPI>( plugin, m_host );
        registerProperty( "video", make_property( this, &JSRootAPI::get_video ) );

        m_mediaDesc = boost::make_shared<JSCurrentMediaDescAPI>( plugin );
        registerProperty( "mediaDescription", make_property( this, &JSRootAPI::get_mediaDesc ) );
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @fn JSRootAPI::~JSRootAPI()
    ///
    /// @brief  Destructor.  Remember that this object will not be released until
    ///         the browser is done with it; this will almost definitely be after
    ///         the plugin is released.
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~JSRootAPI() {};

    ChimeraPtr getPlugin();

    std::string get_version();
    std::string get_vlcVersion();

    void play( const std::string& mrl );
    void pause();
    void togglePause();
    void stop();

    bool get_playing();

    double get_length();

    double get_position();
    void set_position( double );

    double get_time();
    void set_time( double );

    unsigned int get_volume();
    void set_volume( unsigned int );

    void toggleMute();

    std::string get_bgcolor();
    void set_bgcolor( const std::string& bg );

    bool get_fullscreen();
    void set_fullscreen( bool fs );

    int get_state();

    void toggleFullscreen();

    JSAudioAPIWeakPtr     get_audio()     { return m_audio; }
    JSInputAPIWeakPtr     get_input()     { return m_input; }
    JSPlaylistAPIWeakPtr  get_playlist()  { return m_playlist; }
    JSSubtitleAPIWeakPtr  get_subtitle()  { return m_subtitle; }
    JSVideoAPIWeakPtr     get_video()     { return m_video; }
    JSMediaDescAPIWeakPtr get_mediaDesc() { return m_mediaDesc; }

    /* async events from libvlc */
    FB_JSAPI_EVENT( MediaPlayerMediaChanged, 0, () );
    FB_JSAPI_EVENT( MediaPlayerNothingSpecial, 0, () );
    FB_JSAPI_EVENT( MediaPlayerOpening, 0, () );
    FB_JSAPI_EVENT( MediaPlayerBuffering, 1, ( float ) );
    FB_JSAPI_EVENT( MediaPlayerPlaying, 0, () );
    FB_JSAPI_EVENT( MediaPlayerPaused, 0, () );
    FB_JSAPI_EVENT( MediaPlayerForward, 0, () );
    FB_JSAPI_EVENT( MediaPlayerBackward, 0, () );
    FB_JSAPI_EVENT( MediaPlayerEncounteredError, 0, () );
    FB_JSAPI_EVENT( MediaPlayerEndReached, 0, () );
    FB_JSAPI_EVENT( MediaPlayerStopped, 0, () );

    FB_JSAPI_EVENT( MediaPlayerTimeChanged, 1, ( double ) );
    FB_JSAPI_EVENT( MediaPlayerPositionChanged, 1, ( float ) );
    FB_JSAPI_EVENT( MediaPlayerSeekableChanged, 1, ( bool ) );
    FB_JSAPI_EVENT( MediaPlayerPausableChanged, 1, ( bool ) );
    FB_JSAPI_EVENT( MediaPlayerLengthChanged, 1, ( double ) );

private:
    ChimeraWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    JSAudioAPIPtr            m_audio;
    JSInputAPIPtr            m_input;
    JSPlaylistAPIPtr         m_playlist;
    JSSubtitleAPIPtr         m_subtitle;
    JSVideoAPIPtr            m_video;
    JSCurrentMediaDescAPIPtr m_mediaDesc;
};

#endif // H_CHIMERA_API
