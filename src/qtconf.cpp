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

#include "qtconf.h"

#include <cstdint>
#include <sstream>
#include <iomanip>

#include <boost/algorithm/string/replace.hpp>

#include <QCoreApplication>
#include <QtEndian>

#include <utf8_tools.h>

static std::string qtConf_resource_data;

static const unsigned char qtConf_resource_name[] = {
    // qt
    0x0,0x2,
    0x0,0x0,0x7,0x84,
    0x0,0x71,
    0x0,0x74,
    // etc
    0x0,0x3,
    0x0,0x0,0x6c,0xa3,
    0x0,0x65,
    0x0,0x74,0x0,0x63,
    // qt.conf
    0x0,0x7,
    0x8,0x74,0xa6,0xa6,
    0x0,0x71,
    0x0,0x74,0x0,0x2e,0x0,0x63,0x0,0x6f,0x0,0x6e,0x0,0x66,
};

static const unsigned char qtConf_resource_struct[] = {
    // :
    0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x1,
    // :/qt
    0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x2,
    // :/qt/etc
    0x0,0x0,0x0,0xa,0x0,0x2,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x3,
    // :/qt/etc/qt.conf
    0x0,0x0,0x0,0x16,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,
};

QT_BEGIN_NAMESPACE
extern Q_CORE_EXPORT
bool qRegisterResourceData( int, const unsigned char*,
                            const unsigned char* ,
                            const unsigned char* );
extern Q_CORE_EXPORT
bool qUnregisterResourceData( int, const unsigned char*,
                              const unsigned char*,
                              const unsigned char* );
QT_END_NAMESPACE

void InitQtConf( const std::string& prefix )
{
    Q_ASSERT( !qApp );
    if( !qApp ) {
        std::wstring qtPrefix =
            FB::utf8_to_wstring( prefix );
        boost::algorithm::replace_all( qtPrefix, L"\\", L"/" );

        std::stringstream escPrefixStream;
        for( wchar_t c : qtPrefix ) {
            if( c > 0xff )
                escPrefixStream << "\\x" << std::setfill( '0' ) << std::setw( 4 ) << std::hex << c;
            else
                escPrefixStream << static_cast<std::string::value_type>( c );;
        }

        qtConf_resource_data = "4321[Paths]\n";
        qtConf_resource_data += "Prefix = " + escPrefixStream.str() + "\n";
        uint32_t qtConfSize = qtConf_resource_data.size() - sizeof( qtConfSize );
        uint32_t qtConfSwappedSize = qToBigEndian( qtConfSize );
        memcpy( &qtConf_resource_data[0], &qtConfSwappedSize, sizeof( qtConfSwappedSize ) );

        qRegisterResourceData( 0x01, qtConf_resource_struct,
                               qtConf_resource_name,
                               (const unsigned char*)qtConf_resource_data.data() );
    }
}
