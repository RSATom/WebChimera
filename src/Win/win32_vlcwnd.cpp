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

#include "win32_vlcwnd.h"

LRESULT CALLBACK VLCWnd::_WindowProc(HWND hWnd, UINT uMsg,
                                     WPARAM wParam, LPARAM lParam)
{
    LONG_PTR ud = GetWindowLongPtr(hWnd, GWLP_USERDATA);
    VLCWnd* wnd = reinterpret_cast<VLCWnd*>(ud);

    if( !wnd && WM_CREATE != uMsg )
        return DefWindowProc(hWnd, uMsg, wParam, lParam);

    switch( uMsg )
    {
        case WM_CREATE:{
            CREATESTRUCT* cs = (CREATESTRUCT*)(lParam);
            wnd = reinterpret_cast<VLCWnd*>(cs->lpCreateParams);
            wnd->_hWnd = hWnd;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)wnd);
            return wnd->WindowProc(uMsg, wParam, lParam);
        }
        case WM_NCDESTROY:{
            LRESULT r = wnd->WindowProc(uMsg, wParam, lParam);
            wnd->_hWnd = 0;
            return r;
        }
        default:{
            return wnd->WindowProc(uMsg, wParam, lParam);
       }
    }
}

bool VLCWnd::RegisterClass(WNDCLASS* wc)
{
    memset(wc, 0, sizeof(WNDCLASS));

    wc->style       = CS_DBLCLKS;
    wc->hCursor     = LoadCursor(NULL, IDC_ARROW);
    wc->lpfnWndProc = _WindowProc;
    wc->hInstance   = _hInstance;

    PreRegisterWindowClass(wc);

    if( !wc->lpszClassName )
        return false;

    if( GetClassInfo(_hInstance, wc->lpszClassName, wc) ) {
        return true;
    } else {
        _wndclass_atom = ::RegisterClass(wc);
        return _wndclass_atom != 0;
    }
}

bool VLCWnd::Create(LPCTSTR lpWindowName, DWORD dwStyle,
                    int x, int y, int nWidth, int nHeight,
                    HWND hWndParent, HMENU hMenu)
{
    return CreateEx(0, lpWindowName, dwStyle,
                    x, y, nWidth, nHeight,
                    hWndParent, hMenu);
}

bool VLCWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpWindowName, DWORD dwStyle,
                      int x, int y, int nWidth, int nHeight,
                      HWND hWndParent, HMENU hMenu)
{
    if(_hWnd)
        return false;

    WNDCLASS wc;
    if( !RegisterClass(&wc) )
        return false;

    ::CreateWindowEx(dwExStyle, wc.lpszClassName, lpWindowName, dwStyle,
                     x, y, nWidth, nHeight,
                     hWndParent, hMenu, _hInstance, (LPVOID)this);

    return _hWnd != 0;
}

VLCWnd::~VLCWnd()
{
    if( _hWnd )
        DestroyWindow( _hWnd );

    if( 0 != _wndclass_atom ) {
        if( UnregisterClass(MAKEINTATOM(_wndclass_atom), _hInstance) )
            _wndclass_atom = 0;
    }
}
