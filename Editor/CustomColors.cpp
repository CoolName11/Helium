#include "EditorPch.h"
#include "CustomColors.h"

#include "Foundation/Tokenize.h"

#include <sstream>

using namespace Helium;

static const tstring s_ColorDelimiter = TXT( "#" );

///////////////////////////////////////////////////////////////////////////////
// Gets the registry key for where the custom colors should be stored by default.
// 
tstring CustomColors::GetConfigKey()
{
    return TXT( "CustomColors" );
}

///////////////////////////////////////////////////////////////////////////////
// Iterates over all the custom colors in colorData and builds a delimited
// string representation of the information.  This information can be persisted
// by the application and reloaded later by calling Load.
// 
tstring CustomColors::Save( wxColourData& colorData )
{
    tostringstream result;

    for ( int32_t colorIndex = 0; colorIndex < NumColors; ++colorIndex )
    {
        if ( colorIndex > 0 )
        {
            result << s_ColorDelimiter;
        }

        const wxColour& color = colorData.GetCustomColour( colorIndex );
        if ( color.IsOk() )
        {
            // GetAsString returns HTML format (example: #FF00FF).  Strip the leading #.
            result << color.GetAsString( wxC2S_HTML_SYNTAX ).substr( 1 ).c_str();
        }
    }

    return result.str();
}

///////////////////////////////////////////////////////////////////////////////
// Populates the custom colors of colorData with the values stored in the info
// string.  The info string is expected to have been generated by calling the
// above Save function.
// 
void CustomColors::Load( wxColourData& colorData, const tstring& info )
{
    if ( !info.empty() )
    {
        std::vector< tstring > colors;
        Tokenize( info, colors, s_ColorDelimiter );

        std::vector< tstring >::const_iterator colorItr = colors.begin();
        std::vector< tstring >::const_iterator colorEnd = colors.end();
        for ( int32_t colorIndex = 0; colorItr != colorEnd && colorIndex < NumColors; ++colorItr, ++colorIndex )
        {
            const tstring& colorStr = *colorItr;
            if ( colorStr.length() >= 6 )
            {
                uint32_t red = 0;
                uint32_t green = 0;
                uint32_t blue = 0;
#pragma TODO("What is the std library equivalent of this?")
#if HELIUM_WCHAR_T
                swscanf( colorStr.c_str(), TXT( "%02X%02X%02X" ), &red, &green, &blue );
#else
                sscanf( colorStr.c_str(), TXT( "%02X%02X%02X" ), &red, &green, &blue );
#endif
                colorData.SetCustomColour( colorIndex, wxColour( (uint8_t)red, (uint8_t)green, (uint8_t)blue ) );
            }
        }
    }
}
