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

#ifndef _WIN32_VLCWND_H_
#define _WIN32_VLCWND_H_

#include <windows.h>

class VLCWnd
{
protected:
    VLCWnd(HINSTANCE hInstance)
        : _hInstance(hInstance), _wndclass_atom(0), _hWnd(0){};
    virtual ~VLCWnd();

    bool Create(LPCTSTR lpWindowName, DWORD dwStyle,
                int x, int y, int nWidth, int nHeight,
                HWND hWndParent, HMENU hMenu);
    bool CreateEx(DWORD dwExStyle, LPCTSTR lpWindowName, DWORD dwStyle,
                  int x, int y, int nWidth, int nHeight,
                  HWND hWndParent, HMENU hMenu);

    virtual void PreRegisterWindowClass(WNDCLASS* ) {};
    virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
        { return DefWindowProc(_hWnd, uMsg, wParam, lParam); };

    HINSTANCE hInstance() const
        { return _hInstance;}

public:
    HWND hWnd() const {return _hWnd;}

private:
    bool RegisterClass(WNDCLASS* wc);

private:
    static LRESULT CALLBACK _WindowProc(HWND hWnd, UINT uMsg, WPARAM, LPARAM);
    HINSTANCE _hInstance;
    ATOM _wndclass_atom;
    HWND _hWnd;
};

#endif //_WIN32_VLCWND_H_
