/*****************************************************************************
 * Copyright (c) 2012-2014 Sergey Radionov <rsatom_gmail.com>
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

 #include "FBVLC_X11.h"

////////////////////////////////////////////////////////////////////////////////
//FBVLC_X11 class
////////////////////////////////////////////////////////////////////////////////
FBVLC_X11::FBVLC_X11()
    : m_xcbConnection( 0 ), m_xcbContextId( 0 ), m_frameBuf( 0 )
{
}

FBVLC_X11::~FBVLC_X11()
{
}

bool FBVLC_X11::onWindowAttached( FB::AttachedEvent*, FB::PluginWindowX11* w )
{
    vlc_open();
    libvlc_media_player_set_xwindow( get_player().get_mp(), w->getWindow() );
    return true;
}

bool FBVLC_X11::onWindowDetached( FB::DetachedEvent*, FB::PluginWindowX11* )
{
    vlc_close();
    return true;
}

bool FBVLC_X11::onWindowAttached( FB::AttachedEvent*, X11WindowlessWindow* w )
{
    vlc_open();

    m_xcbConnection = XGetXCBConnection( w->getDisplay() );
    m_xcbContextId = xcb_generate_id( m_xcbConnection );
    xcb_create_gc( m_xcbConnection, m_xcbContextId, w->getBrowserWindow(), 0, 0 );

    updateBgColor();

    return true;
}

bool FBVLC_X11::onWindowDetached( FB::DetachedEvent*, X11WindowlessWindow* )
{
    vlc_close();

    xcb_free_gc( m_xcbConnection, m_xcbContextId );
    m_xcbContextId = 0;
    m_xcbConnection = 0;

    return true;
}

void FBVLC_X11::on_frame_ready( const std::vector<char>* frameBuf )
{
    if( m_frameBuf != frameBuf ) {
        m_frameGuard.lock();
        m_frameBuf = frameBuf;
        m_frameGuard.unlock();
    }

    updateWindow();
}

void FBVLC_X11::on_frame_cleanup()
{
    m_frameGuard.lock();
    m_frameBuf = 0;
    m_frameGuard.unlock();

    updateWindow();
}

void FBVLC_X11::fillBackground( xcb_drawable_t drawable,
                                const xcb_rectangle_t* bgRects,
                                uint32_t rectCount )
{
    xcb_poly_fill_rectangle( m_xcbConnection, drawable,
                             m_xcbContextId, rectCount, bgRects );
}

bool FBVLC_X11::onExposeEvent( X11ExposeEvent* event, X11WindowlessWindow* w )
{
    if( !m_xcbConnection )
        return false;

    assert( event->display == w->getDisplay() );

    const FB::Rect outRect = w->getWindowPosition();
    const uint32_t outWidth = rectWidth( outRect );
    const uint32_t outHeight = rectHeight( outRect );

    boost::lock_guard<boost::mutex> lock( m_frameGuard );

    const unsigned mediaWidth = vlc::vmem::width();
    const unsigned mediaHeight = vlc::vmem::height();
    const unsigned frameSize = mediaWidth * mediaHeight * vlc::DEF_PIXEL_BYTES;

    if( m_frameBuf ) {
        assert( m_frameBuf->size() >= frameSize );

        const int16_t dstX = outRect.left + ( outWidth - mediaWidth ) / 2;
        const int16_t dstY = outRect.top + ( outHeight - mediaHeight ) / 2;

        xcb_put_image( m_xcbConnection, XCB_IMAGE_FORMAT_Z_PIXMAP,
                       event->drawable, m_xcbContextId,
                       mediaWidth, mediaHeight,
                       dstX, dstY, 0, 24,
                       frameSize,
                       (const uint8_t*)&(*m_frameBuf)[0] );

        if( mediaWidth < outWidth ) {
            const xcb_rectangle_t bgBorders[2] = {
                { outRect.left, outRect.top, dstX - outRect.left, outHeight },
                { dstX + mediaWidth, outRect.top, outWidth - ( dstX + mediaWidth ), outHeight }
            };
            fillBackground( event->drawable, bgBorders, 2 );
        } else if( mediaHeight < outHeight ) {
            const xcb_rectangle_t bgBorders[2] = {
                { outRect.left, outRect.top, outWidth, dstY - outRect.top },
                { outRect.left, dstY + mediaHeight, outWidth, outRect.bottom - ( dstY + mediaHeight ) }
            };
            fillBackground( event->drawable, bgBorders, 2 );
        }
    } else {
        const xcb_rectangle_t bgRect = { outRect.left, outRect.top, outWidth, outHeight };
        fillBackground( event->drawable, &bgRect, 1 );
    }

    xcb_flush( m_xcbConnection );

    return true;
}

void FBVLC_X11::updateWindow()
{
    if( FB::PluginWindow* w = GetWindow() ) {
        w->InvalidateWindow();
    }
}

void FBVLC_X11::updateBgColor()
{
    if( !isWindowless() )
        return;

    if( X11WindowlessWindow* w = static_cast<X11WindowlessWindow*>( GetWindow() ) ) {
        uint8_t r = 0, g = 0, b = 0;
        HtmlColor2RGB( get_options().get_bg_color(), &r, &g, &b );

        xcb_alloc_color_cookie_t colorCookie =
            xcb_alloc_color( m_xcbConnection, w->getColormap(),
                             uint16_t( r ) << 8,
                             uint16_t( g ) << 8,
                             uint16_t( b ) << 8 );

        xcb_alloc_color_reply_t* colorReply =
                xcb_alloc_color_reply( m_xcbConnection, colorCookie, 0 );

        if( colorReply ) {
            uint32_t mask = XCB_GC_FOREGROUND;
            uint32_t values[] = { colorReply->pixel };

            xcb_change_gc( m_xcbConnection, m_xcbContextId, mask, values );

            free( colorReply );
        }
    }
}

void FBVLC_X11::on_option_change( vlc_player_option_e option )
{
    FBVLC::on_option_change(option);

    switch (option) {
        case po_bg_color: {
            updateBgColor();
            updateWindow();
            break;
        }
        default:
            break;
    }
}
