#pragma once

#include <string>
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#endif

bool HtmlColor2RGB( const std::string& HtmlColor,
                    uint8_t* r, uint8_t* g, uint8_t* b );

#ifdef _WIN32
inline COLORREF HtmlColor2RGB( const std::string& HtmlColor,
                               COLORREF DefColor )
{
    uint8_t r, g, b; r = g = b = 0;

    if( !HtmlColor2RGB( HtmlColor, &r, &g, &b ) )
        return DefColor;

    return RGB( r, g, b );
}
#endif

