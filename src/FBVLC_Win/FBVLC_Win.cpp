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

#include "FBVLC_Win.h"
#include "resource.h"

#include <boost/thread/mutex.hpp>

////////////////////////////////////////////////////////////////////////////////
//WindowedWM class
////////////////////////////////////////////////////////////////////////////////
WindowedWM::WindowedWM( HMODULE hDllModule, vlc_player_options* po )
    :VLCWindowsManager( hDllModule, m_rc, po )
{
    m_rc.hBackgroundIcon =
        (HICON) LoadImage( hDllModule, MAKEINTRESOURCE( IDI_BG_ICON ),
                           IMAGE_ICON, 0, 0, LR_DEFAULTSIZE );
}

////////////////////////////////////////////////////////////////////////////////
//FBVLC_Win class
////////////////////////////////////////////////////////////////////////////////
FBVLC_Win::FBVLC_Win()
    : m_hBgBrush( NULL ), m_frame_buf( 0 )
{
    vlc_player_options& o = get_options();

    COLORREF bg_color = HtmlColor2RGB( o.get_bg_color(), RGB( 0, 0, 0 ) );
    m_hBgBrush = CreateSolidBrush( bg_color );
}

FBVLC_Win::~FBVLC_Win()
{
    DeleteObject( m_hBgBrush );
}

bool FBVLC_Win::onRefreshEvent( FB::RefreshEvent *evt, FB::PluginWindowlessWin* w )
{
    HDC hDC = w->getHDC();
    FB::Rect fbRect = evt->bounds;

    RECT Rect = { fbRect.left, fbRect.top, fbRect.right, fbRect.bottom };
    FillRect( hDC, &Rect, m_hBgBrush );

    boost::lock_guard<boost::mutex> lock( m_frame_guard );

    const unsigned media_width = vlc::vmem::width();
    const unsigned media_height = vlc::vmem::height();

    if( m_frame_buf ) {
        assert( m_frame_buf->size() >= media_width * media_height * vlc::DEF_PIXEL_BYTES );

        BITMAPINFO BmpInfo; ZeroMemory( &BmpInfo, sizeof( BmpInfo ) );
        BITMAPINFOHEADER& BmpH = BmpInfo.bmiHeader;
        BmpH.biSize = sizeof( BITMAPINFOHEADER );
        BmpH.biWidth = media_width;
        BmpH.biHeight = -( ( int ) media_height );
        BmpH.biPlanes = 1;
        BmpH.biBitCount = vlc::DEF_PIXEL_BYTES * 8;
        BmpH.biCompression = BI_RGB;
        //following members are already zeroed
        //BmpH.biSizeImage = 0;
        //BmpH.biXPelsPerMeter = 0;
        //BmpH.biYPelsPerMeter = 0;
        //BmpH.biClrUsed = 0;
        //BmpH.biClrImportant = 0;


        FB::Rect wrect;
        if( getBrowser() == "IE" )
            wrect = fbRect;
        else
            wrect = w->getWindowPosition();

        if( get_options().get_native_scaling() ) {
            const float src_aspect = ( float ) media_width / media_height;
            const float dst_aspect = ( float ) w->getWindowWidth() / w->getWindowHeight();
            unsigned dst_media_width = w->getWindowWidth();
            unsigned dst_media_height = w->getWindowHeight();
            if( src_aspect > dst_aspect ) {
                if( w->getWindowWidth() != media_width ) { //don't scale if size equal
                    dst_media_height = static_cast<unsigned>( w->getWindowWidth() / src_aspect + 0.5 );
                }
            }
            else {
                if( w->getWindowHeight() != media_height ) { //don't scale if size equal
                    dst_media_width = static_cast<unsigned>( w->getWindowHeight() * src_aspect + 0.5 );
                }
            }

            SetStretchBltMode( hDC, COLORONCOLOR );
            BOOL r =
                StretchDIBits( hDC,
                               wrect.left + ( w->getWindowWidth() - dst_media_width ) / 2,
                               wrect.top + ( w->getWindowHeight() - dst_media_height ) / 2,
                               dst_media_width, dst_media_height,
                               0, 0,
                               media_width, media_height,
                               &( *m_frame_buf )[0],
                               &BmpInfo, DIB_RGB_COLORS, SRCCOPY );
        } else {
            BOOL r =
                SetDIBitsToDevice( hDC,
                                   wrect.left + ( w->getWindowWidth() - media_width ) / 2,
                                   wrect.top + ( w->getWindowHeight() - media_height ) / 2,
                                   media_width, media_height,
                                   0, 0,
                                   0, media_height,
                                   &( *m_frame_buf )[0],
                                   &BmpInfo, DIB_RGB_COLORS );
        }
    }

    return true;
}

void FBVLC_Win::on_option_change( vlc_player_option_e option )
{
    FBVLC::on_option_change( option );

    vlc_player_options& o = get_options();

    switch( option ) {
        case po_bg_color: {
            if( isWindowless() ) {
                HBRUSH hTmpBrush = m_hBgBrush;
                COLORREF bg_color = HtmlColor2RGB( o.get_bg_color(), RGB( 0, 0, 0 ) );
                m_hBgBrush = CreateSolidBrush( bg_color );
                DeleteObject( hTmpBrush );

                if( GetWindow() )
                    GetWindow()->InvalidateWindow();
            }
            break;
        }
        default:
            break;
    }
}

bool FBVLC_Win::onWindowAttached( FB::AttachedEvent* evt, FB::PluginWindowWin* w )
{
    m_wm.reset( new WindowedWM( GetModuleHandleA( getFSPath().c_str() ), &get_options() ) );
    m_wm->CreateWindows( w->getHWND() );
    vlc_open();
    m_wm->LibVlcAttach( &get_player() );
    return true;
}

bool FBVLC_Win::onWindowDetached( FB::DetachedEvent* evt, FB::PluginWindowWin* w )
{
    m_wm->LibVlcDetach();
    vlc_close();
    m_wm->DestroyWindows();
    return true;
}

bool FBVLC_Win::onWindowResized( FB::ResizedEvent* evt, FB::PluginWindowWin* w )
{
    VLCWnd* child = m_wm->getHolderWnd();
    if( child ) {
        RECT rect;
        GetClientRect( w->getHWND(), &rect );
        MoveWindow( child->hWnd(),
                    0, 0,
                    rect.right-rect.left, rect.bottom-rect.top,
                    TRUE );
    };

    return true;
}

bool FBVLC_Win::is_fullscreen()
{
    if( isWindowless() ) {
        return false;//fullscreen mode not supported in windowless mode for now
    } else {
        return m_wm.get() ? m_wm->IsFullScreen() : false ;
    }
}

void FBVLC_Win::set_fullscreen( bool fs )
{
    //fullscreen mode not supported in windowless mode for now
    if( !isWindowless() && m_wm.get() ) {
        if( !m_wm->IsFullScreen() && fs ) {
            m_wm->StartFullScreen();
        } else if( m_wm->IsFullScreen() && !fs ) {
            m_wm->EndFullScreen();
        }
    }
}
void FBVLC_Win::toggle_fullscreen()
{
    //fullscreen mode not supported in windowless mode for now
    if( !isWindowless() && m_wm.get() ) {
        m_wm->ToggleFullScreen();
    }
}

void FBVLC_Win::update_window()
{
    FB::PluginWindow* w = GetWindow();
    if( w ) {
        w->InvalidateWindow();
    }
}

void FBVLC_Win::on_frame_ready( const std::vector<char>* frame_buf )
{
    if( m_frame_buf != frame_buf ) {
        m_frame_guard.lock();
        m_frame_buf = frame_buf;
        m_frame_guard.unlock();
    }

    update_window();
}

void FBVLC_Win::on_frame_cleanup()
{
    m_frame_guard.lock();
    m_frame_buf = 0;
    m_frame_guard.unlock();

    update_window();
}
