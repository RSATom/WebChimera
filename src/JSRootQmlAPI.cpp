/*****************************************************************************
 * Copyright © 2014 Sergey Radionov <rsatom_gmail.com>
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

#include "JSRootQmlAPI.h"

std::string JSRootQmlAPI::get_qmlError()
{
    QmlChimeraPtr plg = boost::static_pointer_cast<QmlChimera>( getPlugin() );

    return plg->getQmlError();
}

std::string JSRootQmlAPI::get_qml()
{
    ChimeraPtr plg = getPlugin();
    const vlc_player_options& o = plg->get_options();

    return o.get_qml();
}

void JSRootQmlAPI::set_qml( const std::string& qml )
{
    ChimeraPtr plg = getPlugin();
    vlc_player_options& o = plg->get_options();

    o.set_qml( qml );
}

void JSRootQmlAPI::emitJsMessage( const std::string& message )
{
    QmlChimeraPtr plg = boost::static_pointer_cast<QmlChimera>( getPlugin() );

    plg->emitJsMessage( message );
}
