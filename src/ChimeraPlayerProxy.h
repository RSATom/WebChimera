/*****************************************************************************
 * Copyright � 2015 Sergey Radionov <rsatom_gmail.com>
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

#include <QmlVlc/QmlVlcPlayerProxy.h>

class ChimeraPlayerProxy : public QmlVlcPlayerProxy
{
public:
    explicit ChimeraPlayerProxy( const std::shared_ptr<vlc::playlist_player_core>& player,
                                 QObject* parent );
    ~ChimeraPlayerProxy();
};
