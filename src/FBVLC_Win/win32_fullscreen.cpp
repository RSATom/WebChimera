/*****************************************************************************
 * Copyright (c) 2011-2014 Sergey Radionov <rsatom_gmail.com>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <windows.h>
#include <commctrl.h>
#include <uxtheme.h>

#include "win32_fullscreen.h"

////////////////////////////////////////////////////////////////////////////////
//VLCHolderWnd members
////////////////////////////////////////////////////////////////////////////////
enum{
    WM_TRY_SET_MOUSE_HOOK = WM_USER+1,
    WM_MOUSE_EVENT_NOTIFY = WM_APP+1,
    WM_MOUSE_EVENT_NOTIFY_SUCCESS = 0xFF
};

VLCHolderWnd*
VLCHolderWnd::CreateHolderWindow( HINSTANCE hInstance,
                                  HWND hParentWnd, VLCWindowsManager* WM )
{
    VLCHolderWnd* wnd = new VLCHolderWnd( hInstance, WM );
    if( wnd && wnd->Create( hParentWnd ) ) {
        return wnd;
    }
    delete wnd;
    return 0;
}

VLCHolderWnd::~VLCHolderWnd()
{
    if( _hBgBrush ) {
        DeleteObject( _hBgBrush );
        _hBgBrush = 0;
    }
}

bool VLCHolderWnd::Create( HWND hWndParent )
{
    return VLCWnd::Create( TEXT("Holder Window"),
                           WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
                           0, 0, 0, 0, hWndParent, 0 );
}

void VLCHolderWnd::PreRegisterWindowClass( WNDCLASS* wc )
{
    if( !_hBgBrush ) {
        //if bg color string can't be parsed, then black color will be used
        _hBgBrush =
            CreateSolidBrush( HtmlColor2RGB( PO()->get_bg_color(),
                              RGB( 0, 0, 0 ) ) );
    }

    wc->hbrBackground = _hBgBrush;
    wc->lpszClassName = TEXT("Web Plugin VLC Window Holder Class");
}

LRESULT VLCHolderWnd::WindowProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        case WM_CREATE:{
            CREATESTRUCT* CreateStruct = (CREATESTRUCT*) lParam;

            RECT ParentClientRect;
            GetClientRect( CreateStruct->hwndParent, &ParentClientRect );
            MoveWindow( hWnd(), 0, 0,
                        ( ParentClientRect.right-ParentClientRect.left ),
                        ( ParentClientRect.bottom-ParentClientRect.top ), FALSE );
            break;
        }
        case WM_PAINT:{
            PAINTSTRUCT PaintStruct;
            HDC hDC = BeginPaint( hWnd(), &PaintStruct );
            RECT rect;
            GetClientRect( hWnd(), &rect );
            int IconX = ( ( rect.right - rect.left ) - GetSystemMetrics( SM_CXICON ) ) / 2 ;
            int IconY = ( ( rect.bottom - rect.top ) - GetSystemMetrics( SM_CYICON ) ) / 2;
            DrawIcon( hDC, IconX, IconY, RC().hBackgroundIcon );
            EndPaint( hWnd(), &PaintStruct );
            break;
        }
        case WM_SHOWWINDOW:{
            break;
        }
        case WM_TRY_SET_MOUSE_HOOK: {
            MouseHook( true );
            break;
        }
        case WM_SIZE:
            break;
        case WM_MOUSEMOVE:
        case WM_LBUTTONDBLCLK:
            WM().OnMouseEvent( uMsg );
            break;
        case WM_MOUSE_EVENT_NOTIFY: {
            WM().OnMouseEvent( wParam );
            return WM_MOUSE_EVENT_NOTIFY_SUCCESS;
        }
        default:
            return VLCWnd::WindowProc( uMsg, wParam, lParam );
    }
    return 0;
}

void VLCHolderWnd::DestroyWindow()
{
    LibVlcDetach();

    if( hWnd() )
        ::DestroyWindow( hWnd() );
};

LRESULT CALLBACK VLCHolderWnd::MouseHookProc( int nCode, WPARAM wParam, LPARAM lParam )
{
    bool AllowReceiveMessage = true;
    if( nCode >= 0 ) {
        switch( wParam ) {
            case WM_MOUSEMOVE:
            case WM_LBUTTONDBLCLK: {
                MOUSEHOOKSTRUCT* mhs = reinterpret_cast<MOUSEHOOKSTRUCT*>( lParam );

                //try to find HolderWnd and notify it
                HWND hNotifyWnd = mhs->hwnd;
                LRESULT SMRes =
                    ::SendMessage( hNotifyWnd, WM_MOUSE_EVENT_NOTIFY, wParam, 0 );
                while( hNotifyWnd && WM_MOUSE_EVENT_NOTIFY_SUCCESS != SMRes ) {
                    hNotifyWnd = GetParent( hNotifyWnd );
                    SMRes = ::SendMessage( hNotifyWnd, WM_MOUSE_EVENT_NOTIFY, wParam, 0 );
                }

                AllowReceiveMessage =
                    WM_MOUSEMOVE == wParam ||
                    ( WM_MOUSE_EVENT_NOTIFY_SUCCESS != SMRes );
                break;
            }
        }
    }

    LRESULT NHRes = CallNextHookEx( NULL, nCode, wParam, lParam );
    if( AllowReceiveMessage )
        return NHRes;
    else
        return 1;
}

void VLCHolderWnd::MouseHook( bool SetHook )
{
    if( SetHook ) {
        HWND hMPWnd = FindMP_hWnd();
        const DWORD WndThreadID =
            hMPWnd ? GetWindowThreadProcessId( hMPWnd, NULL ) : 0;
        if( _hMouseHook && ( !hMPWnd || WndThreadID != _MouseHookThreadId ) ) {
            //unhook if something changed
            MouseHook( false );
        }

        if( !_hMouseHook && hMPWnd && WndThreadID ) {
            _MouseHookThreadId = WndThreadID;
            _hMouseHook =
                SetWindowsHookEx( WH_MOUSE, VLCHolderWnd::MouseHookProc,
                                  NULL, WndThreadID );
        }
    }
    else{
        if( _hMouseHook ) {
            UnhookWindowsHookEx( _hMouseHook );
            _MouseHookThreadId = 0;
            _hMouseHook = 0;
        }
    }
}

HWND VLCHolderWnd::FindMP_hWnd()
{
    return GetWindow( hWnd(), GW_CHILD );
}

//libvlc events arrives from separate thread
void VLCHolderWnd::OnLibVlcEvent( const libvlc_event_t* event )
{
    //We need set hook to catch doubleclicking (to switch to fullscreen and vice versa).
    //But libvlc media window may not exist yet,
    //and we don't know when it will be created, nor ThreadId of it.
    //So we try catch events,
    //(suppose wnd will be ever created),
    //and then try set mouse hook.
    HWND hMPWnd = FindMP_hWnd();
    const DWORD WndThreadID =
        hMPWnd ? GetWindowThreadProcessId( hMPWnd, NULL ) : 0;
    //if no hook, or window thread has changed
    if(!_hMouseHook || ( hMPWnd && WndThreadID != _MouseHookThreadId ) ) {
        DWORD s = GetWindowLong( hMPWnd, GWL_STYLE );
        s |= WS_CLIPSIBLINGS;
        SetWindowLong( hMPWnd, GWL_STYLE, s );

        //libvlc events arrives from separate thread,
        //so we need post message to main thread, to notify it.
        PostMessage( hWnd(), WM_TRY_SET_MOUSE_HOOK, 0, 0 );
    }
}

void VLCHolderWnd::LibVlcAttach()
{
    if( VP() )
        libvlc_media_player_set_hwnd( VP()->get_mp(), hWnd() );
}

void VLCHolderWnd::LibVlcDetach()
{
    if( VP() )
        libvlc_media_player_set_hwnd( VP()->get_mp(), 0);

    MouseHook( false );
}

////////////////////////////////////////////////////////////////////////////////
//VLCFullScreenWnd members
////////////////////////////////////////////////////////////////////////////////
HINSTANCE VLCFullScreenWnd::_hinstance = 0;
ATOM VLCFullScreenWnd::_fullscreen_wndclass_atom = 0;

void VLCFullScreenWnd::RegisterWndClassName( HINSTANCE hInstance )
{
    //save hInstance for future use
    _hinstance = hInstance;

    WNDCLASS wClass;

    memset( &wClass, 0 , sizeof( wClass ) );
    if( ! GetClassInfo( _hinstance,  getClassName(), &wClass ) )
    {
        wClass.style          = CS_NOCLOSE | CS_DBLCLKS;
        wClass.lpfnWndProc    = FSWndWindowProc;
        wClass.cbClsExtra     = 0;
        wClass.cbWndExtra     = 0;
        wClass.hInstance      = _hinstance;
        wClass.hIcon          = NULL;
        wClass.hCursor        = LoadCursor( NULL, IDC_ARROW );
        wClass.hbrBackground  = (HBRUSH) (COLOR_3DFACE + 1 );
        wClass.lpszMenuName   = NULL;
        wClass.lpszClassName  = getClassName();

        _fullscreen_wndclass_atom = RegisterClass( &wClass );
    }
    else
    {
        _fullscreen_wndclass_atom = 0;
    }
}
void VLCFullScreenWnd::UnRegisterWndClassName()
{
    if( 0 != _fullscreen_wndclass_atom ) {
        UnregisterClass( MAKEINTATOM( _fullscreen_wndclass_atom ), _hinstance );
        _fullscreen_wndclass_atom = 0;
    }
}

LRESULT CALLBACK VLCFullScreenWnd::FSWndWindowProc( HWND hWnd, UINT uMsg,
                                                    WPARAM wParam, LPARAM lParam )
{
    VLCFullScreenWnd* fs_data =
        reinterpret_cast<VLCFullScreenWnd*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

    switch( uMsg ) {
        case WM_CREATE: {
            CREATESTRUCT* CreateStruct = (CREATESTRUCT*) lParam;
            VLCWindowsManager* WM = (VLCWindowsManager*) CreateStruct->lpCreateParams;

            fs_data = new VLCFullScreenWnd( hWnd, WM );
            SetWindowLongPtr( hWnd, GWLP_USERDATA,
                              reinterpret_cast<LONG_PTR>( fs_data ) );

            break;
        }
        case WM_NCDESTROY:
            delete fs_data;
            SetWindowLongPtr( hWnd, GWLP_USERDATA, 0 );
            break;
        case WM_SHOWWINDOW: {
            if( FALSE==wParam ) { //hiding
                break;
            }

            //simulate lParam for WM_SIZE
            RECT ClientRect;
            GetClientRect( hWnd, &ClientRect );
            lParam = MAKELPARAM( ClientRect.right, ClientRect.bottom );
        }
        case WM_SIZE: {
            if( fs_data->_WindowsManager->IsFullScreen() ) {
                int new_client_width = LOWORD( lParam );
                int new_client_height = HIWORD( lParam );
                VLCHolderWnd* HolderWnd =  fs_data->_WindowsManager->getHolderWnd();
                SetWindowPos( HolderWnd->hWnd(), HWND_BOTTOM, 0, 0,
                              new_client_width, new_client_height,
                             SWP_NOACTIVATE | SWP_NOOWNERZORDER );
            }
            break;
        }
        case WM_KEYDOWN: {
            if( wParam == VK_ESCAPE && fs_data->_WindowsManager->IsFullScreen() ) {
                fs_data->_WindowsManager->EndFullScreen();
            }
            break;
        }
        default:
            return DefWindowProc( hWnd, uMsg, wParam, lParam );
    }
    return 0L;
};

VLCFullScreenWnd* VLCFullScreenWnd::CreateFSWindow( VLCWindowsManager* WM )
{
    HWND hWnd =
        CreateWindow( getClassName(),
                      TEXT("VLC ActiveX Full Screen Window"),
                      WS_POPUP|WS_CLIPCHILDREN,
                      0, 0,
                      GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ),
                      0,
                      0,
                      VLCFullScreenWnd::_hinstance,
                      (LPVOID) WM );
    if( hWnd )
        return reinterpret_cast<VLCFullScreenWnd*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

    return 0;
}

///////////////////////
//VLCWindowsManager
///////////////////////
VLCWindowsManager::VLCWindowsManager( HMODULE hModule, const VLCViewResources& rc,
                                      const vlc_player_options* po )
    : _rc( rc ), _hModule( hModule ), _po( po ), _hWindowedParentWnd( 0 ), _vp( 0 ),
      _HolderWnd( 0 ), _FSWnd( 0 ), Last_WM_MOUSEMOVE_Pos( 0 )
{
    VLCFullScreenWnd::RegisterWndClassName( hModule );
}

VLCWindowsManager::~VLCWindowsManager()
{
    VLCFullScreenWnd::UnRegisterWndClassName();
}

void VLCWindowsManager::CreateWindows( HWND hWindowedParentWnd )
{
    _hWindowedParentWnd = hWindowedParentWnd;

    if( !_HolderWnd ) {
        _HolderWnd =
            VLCHolderWnd::CreateHolderWindow( getHModule(),
                                              hWindowedParentWnd, this );
    }
}

void VLCWindowsManager::DestroyWindows()
{
    if( _HolderWnd ) {
        _HolderWnd->DestroyWindow();
        delete _HolderWnd;
        _HolderWnd = 0;
    }

    if( _FSWnd ) {
        _FSWnd->DestroyWindow();
    }
    _FSWnd = 0;
}

void VLCWindowsManager::LibVlcAttach( vlc_player* vp )
{
    if( !_HolderWnd )
        return; //VLCWindowsManager::CreateWindows was not called

    if( vp && _vp != vp ) {
        LibVlcDetach();
    }

    if( !_vp ) {
        _vp = vp;
        VlcEvents( true );
    }

    _HolderWnd->LibVlcAttach();
}

void VLCWindowsManager::LibVlcDetach()
{
    if( _HolderWnd )
        _HolderWnd->LibVlcDetach();

    if( _vp ) {
        VlcEvents( false );
        _vp = 0;
    }
}

void VLCWindowsManager::StartFullScreen()
{
    if( !_HolderWnd || ( PO() && !PO()->get_enable_fs() ) )
        return; //VLCWindowsManager::CreateWindows was not called

    if( VP() && !IsFullScreen() ) {
        if( !_FSWnd ) {
            _FSWnd= VLCFullScreenWnd::CreateFSWindow( this );
        }

        RECT FSRect = { 0, 0, GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ) };

        HMONITOR hMonitor = MonitorFromWindow( _hWindowedParentWnd, MONITOR_DEFAULTTONEAREST );
        MONITORINFO MonInfo;
        memset( &MonInfo, 0, sizeof( MonInfo ) );
        MonInfo.cbSize = sizeof( MonInfo );
        if( GetMonitorInfo( hMonitor, &MonInfo ) ) {
            FSRect = MonInfo.rcMonitor;
        }

#ifdef _DEBUG
        //to simplify debugging in fullscreen mode
        UINT FSFlags = SWP_NOZORDER;
#else
        UINT FSFlags = 0;
#endif

        SetParent( _HolderWnd->hWnd(), _FSWnd->getHWND() );
        SetWindowPos( _FSWnd->getHWND(), HWND_TOPMOST,
                      FSRect.left, FSRect.top,
                      FSRect.right - FSRect.left, FSRect.bottom - FSRect.top,
                      FSFlags );

        ShowWindow( _FSWnd->getHWND(), SW_SHOW );
    }
}

void VLCWindowsManager::EndFullScreen()
{
    if( !_HolderWnd )
        return; //VLCWindowsManager::CreateWindows was not called

    if( IsFullScreen() ) {
        SetParent( _HolderWnd->hWnd(), _hWindowedParentWnd );

        RECT WindowedParentRect;
        GetClientRect( _hWindowedParentWnd, &WindowedParentRect );
        MoveWindow( _HolderWnd->hWnd(), 0, 0,
                    WindowedParentRect.right, WindowedParentRect.bottom, FALSE );

        ShowWindow( _FSWnd->getHWND(), SW_HIDE );

        if( _FSWnd ) {
            _FSWnd->DestroyWindow();
        }
        _FSWnd = 0;
   }
}

void VLCWindowsManager::ToggleFullScreen()
{
    if( IsFullScreen() ) {
        EndFullScreen();
    } else {
        StartFullScreen();
    }
}

bool VLCWindowsManager::IsFullScreen()
{
    return 0 != _FSWnd && 0 != _HolderWnd &&
           GetParent( _HolderWnd->hWnd() ) == _FSWnd->getHWND();
}

void VLCWindowsManager::OnMouseEvent( UINT uMouseMsg )
{
    switch( uMouseMsg ) {
        case WM_LBUTTONDBLCLK:
            ToggleFullScreen();
            break;
    }
}

//libvlc events arrives from separate thread
void VLCWindowsManager::OnLibVlcEvent_proxy( const libvlc_event_t* event, void *param )
{
    VLCWindowsManager* WM = static_cast<VLCWindowsManager*>( param );
    WM->OnLibVlcEvent( event );
}

void VLCWindowsManager::VlcEvents( bool Attach )
{
    if( !VP() )
        return;

    vlc_player& vp = *VP();

    libvlc_event_manager_t* em =
        libvlc_media_player_event_manager( vp.get_mp() );
    if( !em )
        return;

    for( int e = libvlc_MediaPlayerMediaChanged; e <= libvlc_MediaPlayerVout; ++e ) {
        switch( e ) {
        //case libvlc_MediaPlayerMediaChanged:
        //case libvlc_MediaPlayerNothingSpecial:
        //case libvlc_MediaPlayerOpening:
        //case libvlc_MediaPlayerBuffering:
        case libvlc_MediaPlayerPlaying:
        //case libvlc_MediaPlayerPaused:
        //case libvlc_MediaPlayerStopped:
        //case libvlc_MediaPlayerForward:
        //case libvlc_MediaPlayerBackward:
        //case libvlc_MediaPlayerEndReached:
        //case libvlc_MediaPlayerEncounteredError:
        //case libvlc_MediaPlayerTimeChanged:
        case libvlc_MediaPlayerPositionChanged:
        //case libvlc_MediaPlayerSeekableChanged:
        //case libvlc_MediaPlayerPausableChanged:
        //case libvlc_MediaPlayerTitleChanged:
        //case libvlc_MediaPlayerSnapshotTaken:
        //case libvlc_MediaPlayerLengthChanged:
        //case libvlc_MediaPlayerVout:
            if( Attach )
                libvlc_event_attach( em, e, OnLibVlcEvent_proxy, this );
            else
                libvlc_event_detach( em, e, OnLibVlcEvent_proxy, this );
            break;
        }
    }
}

void VLCWindowsManager::OnLibVlcEvent( const libvlc_event_t* event )
{
    if( _HolderWnd ) _HolderWnd->OnLibVlcEvent( event );
    if( _FSWnd ) _FSWnd->OnLibVlcEvent( event);
}
