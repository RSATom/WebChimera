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

#include "HtmlColorUtils.h"

inline bool IsHexChar( std::string::value_type ch )
{
    if( ( ch >= '0' && ch <= '9' ) ||
        ( ch >= 'a' && ch <= 'f' ) ||
        ( ch >= 'A' && ch <= 'F' ) )
    {
        return true;
    }
    return false;
}

inline uint8_t HexToBYTE( std::string::value_type ch1,
                          std::string::value_type ch2 )
{
    uint8_t r = 0;

    if( ( ch1 >= '0' && ch1 <= '9' ) ) {
        r = ( ch1 - '0' );
    }
    else if( ( ch1 >= 'a' && ch1 <= 'f' ) ) {
        r = ( ch1 - 'a' + 10 );
    }
    else if( ( ch1 >= 'A' && ch1 <= 'F' ) ) {
        r = ( ch1 - 'A' + 10 );
    }
    r <<= 4;

    if ( ( ch2 >= '0' && ch2 <= '9' ) ) {
        r |= ch2 - '0';
    }
    else if ( ( ch2 >= 'a' && ch2 <= 'f' ) ) {
        r |= ch2 - 'a' + 10;
    }
    else if ( ( ch2 >= 'A' && ch2 <= 'F' ) ) {
        r |= ch2 - 'A' + 10;
    }

    return r;
}

bool HtmlColor2RGB( const std::string& HtmlColor,
                    uint8_t* r, uint8_t* g, uint8_t* b )
{
    /*#rrggbb*/
    if( HtmlColor.size() != 7 || HtmlColor[0] != '#' )
        return false;

    for( std::string::size_type p = 1; p < HtmlColor.size(); ++p ) {
        if( !IsHexChar( HtmlColor[p] ) )
            return false;
    }

    *r = HexToBYTE( HtmlColor[1], HtmlColor[2] );
    *g = HexToBYTE( HtmlColor[3], HtmlColor[4] );
    *b = HexToBYTE( HtmlColor[5], HtmlColor[6] );

    return true;
}

