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

#pragma once

#include "ChimeraAPI.h"

#include "QmlChimera.h"

////////////////////////////////////////////////////////////////////////////
/// JSRootQmlAPI
////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR( JSRootQmlAPI)
class JSRootQmlAPI : public JSRootAPI
{
public:
    JSRootQmlAPI( const QmlChimeraPtr& plugin, const FB::BrowserHostPtr& host )
        : JSRootAPI( boost::static_pointer_cast<Chimera>( plugin ), host )
    {
        registerProperty( "qmlError",
                          make_property( this,
                                         &JSRootQmlAPI::get_qmlError ) );

        registerProperty( "qml",
                          make_property( this, &JSRootQmlAPI::get_qml,
                                               &JSRootQmlAPI::set_qml ) );

        registerMethod( "emitJsMessage", make_method( this, &JSRootQmlAPI::emitJsMessage ) );
    }

    virtual ~JSRootQmlAPI() {};

    std::string get_qmlError();

    std::string get_qml();
    void set_qml( const std::string& qml );

    FB_JSAPI_EVENT( QmlMessage, 1, ( std::string ) );
    FB_JSAPI_EVENT( QmlStringMessage, 2, ( int, std::string ) );
    FB_JSAPI_EVENT( QmlNumberMessage, 3, ( int, int, int ) );

    void emitJsMessage( const std::string& message );
};
