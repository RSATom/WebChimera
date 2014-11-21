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

 #include "FBVLC_Mac.h"

////////////////////////////////////////////////////////////////////////////////
//FBVLC_Mac class
////////////////////////////////////////////////////////////////////////////////
FBVLC_Mac::FBVLC_Mac()
    : m_frameBuf( 0 )
{
    updateBgComponents();
}

FBVLC_Mac::~FBVLC_Mac()
{
}

void FBVLC_Mac::updateBgComponents()
{
    uint8_t r = 0, g = 0, b = 0;
    HtmlColor2RGB( get_options().get_bg_color(), &r, &g, &b );
    m_bgComponents[0] = r / 255.f;
    m_bgComponents[1] = g / 255.f;
    m_bgComponents[2] = b / 255.f;
    m_bgComponents[3] = 1.f;
}

void FBVLC_Mac::on_option_change( vlc_player_option_e option )
{
    FBVLC::on_option_change( option );

    switch( option ) {
        case po_bg_color: {
            updateBgComponents();
            if ( GetWindow() )
                GetWindow()->InvalidateWindow();
            break;
        }
        default:
            break;
    }
}

void FBVLC_Mac::on_frame_ready( const std::vector<char>* frame_buf )
{
    if( m_frameBuf != frame_buf ) {
        m_frameGuard.lock();
        m_frameBuf = frame_buf;
        m_frameGuard.unlock();
    }

    FB::PluginWindow* w = GetWindow();
    if( w ) {
        w->InvalidateWindow();
    }
}

void FBVLC_Mac::on_frame_cleanup()
{
    m_frameGuard.lock();
    m_frameBuf = 0;
    m_frameGuard.unlock();

    FB::PluginWindow* w = GetWindow();
    if( w ) {
        w->InvalidateWindow();
    }
}

bool FBVLC_Mac::onCoreGraphicsDraw( FB::CoreGraphicsDraw* evt, FB::PluginWindowMacCG* )
{
    FB::Rect bounds( evt->bounds );
    //FB::Rect clip( evt->clip );
    short width = bounds.right - bounds.left, height = bounds.bottom - bounds.top;

    CGContextRef cgContext( evt->context );

    CGContextSaveGState( cgContext );

    CGContextTranslateCTM( cgContext, 0.0, height );
    CGContextScaleCTM( cgContext, 1.0, -1.0 );

    CGColorSpaceRef cSpace = CGColorSpaceCreateDeviceRGB();
    CGContextSetFillColorSpace( cgContext, cSpace );

    CGColorRef bgColor = CGColorCreate( cSpace, m_bgComponents );
    CGContextSetFillColorWithColor( cgContext, bgColor );

    boost::lock_guard<boost::mutex> lock( m_frameGuard );

    if ( m_frameBuf ) {
        const unsigned mediaWidth = vlc::vmem::width();
        const unsigned mediaHeight = vlc::vmem::height();

        assert( m_frameBuf->size() >= mediaWidth * mediaHeight * vlc::DEF_PIXEL_BYTES );

        CGRect imgRect = {
            { ( width - mediaWidth ) / 2, ( height - mediaHeight ) / 2 },
            { mediaWidth, mediaHeight }
        };

        const std::vector<char>& fb = *m_frameBuf;

        CGContextRef frameBmpCtx =
            CGBitmapContextCreate( (void*)&fb[0], mediaWidth, mediaHeight, 8,
                                   mediaWidth * vlc::DEF_PIXEL_BYTES, cSpace,
                                   kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Little );
        CGImageRef frameImage = CGBitmapContextCreateImage( frameBmpCtx );

        CGContextDrawImage( cgContext, imgRect, frameImage );

        CGImageRelease( frameImage );
        CGContextRelease( frameBmpCtx );

        if( mediaWidth < width ) {
            CGRect bgLeft = {
                { 0, 0 },
                { imgRect.origin.x, height }
            };
            CGContextFillRect(cgContext, bgLeft);

            CGRect bgRight = {
                { imgRect.origin.x + imgRect.size.width, 0 },
                { width - ( imgRect.origin.x + imgRect.size.width ), height }
            };
            CGContextFillRect( cgContext, bgRight );

        } else if( mediaHeight < height ) {
            CGRect bgTop = {
                { 0, 0 },
                { width, imgRect.origin.y }
            };
            CGContextFillRect(cgContext, bgTop);

            CGRect bgBottom = {
                { 0, imgRect.origin.y + imgRect.size.height },
                { width, height - ( imgRect.origin.y + imgRect.size.height ) }
            };
            CGContextFillRect( cgContext, bgBottom );
        }
    } else {
        CGRect cgBounds = {
            { 0, 0 },
            { width, height }
        };
        CGContextFillRect( cgContext, cgBounds );
    }

    CGColorRelease( bgColor );
    CGColorSpaceRelease( cSpace );
    CGContextRestoreGState( cgContext );

    return true; // This is handled
}
