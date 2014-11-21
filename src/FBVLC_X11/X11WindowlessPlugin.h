/*****************************************************************************
 * Copyright (c) 2014 Sergey Radionov <rsatom_gmail.com>
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

#pragma once

#include <X11/NpapiPluginX11.h>

class X11WindowlessWindow; //#include "X11WindowlessWindow.h"

class X11WindowlessPlugin :
    public FB::Npapi::NpapiPluginX11
{
public:
    X11WindowlessPlugin( const FB::Npapi::NpapiBrowserHostPtr& host,
                         const std::string& mimetype );
    ~X11WindowlessPlugin();

    virtual NPError SetWindow( NPWindow* );
    virtual int16_t HandleEvent( void* event );

private:
    NPWindow* m_npWindow;
    boost::scoped_ptr<X11WindowlessWindow> m_pluginWindow;
};
