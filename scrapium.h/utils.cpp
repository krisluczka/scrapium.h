#include "scrapium.h"

namespace scrapium {
    bool unicode_escape = false;
    bool browser_emulation = true;
    std::string user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36";

    std::string deunicode( char32_t unicode_char ) {
        std::stringstream ss;
        ss << "\\u" << std::setfill( '0' ) << std::setw( 4 ) << std::hex << std::uppercase << static_cast<int>(unicode_char);
        return ss.str();
    }

    std::string deunicode_convert( const std::string& input ) {
        std::string result;
        for ( size_t i( 0 ); i < input.length(); ++i ) {
            unsigned char c( input[i] );
            if ( c < 128 ) {
                // if ascii we good
                result += c;
            } else {
                // unicode escape
                char32_t unicode_char( 0 );
                uint_fast8_t bytes_to_read( 0 );

                if ( (c & 0xE0) == 0xC0 ) {
                    unicode_char = c & 0x1F;
                    bytes_to_read = 1;
                } else if ( (c & 0xF0) == 0xE0 ) {
                    unicode_char = c & 0x0F;
                    bytes_to_read = 2;
                } else if ( (c & 0xF8) == 0xF0 ) {
                    unicode_char = c & 0x07;
                    bytes_to_read = 3;
                }

                for ( uint_fast8_t j( 0 ); j < bytes_to_read; ++j )
                    unicode_char = (unicode_char << 6) | (input[++i] & 0x3F);

                result += deunicode( unicode_char );
            }
        }
        return result;
    }

    std::pair<std::string, std::string> split_url( const std::string& url ) {
        // regex for URLs
        std::regex urlRegex( R"((https?://)?([^/]+)(/.*)?)" );
        std::smatch match;

        // matching the regex
        if ( std::regex_match( url, match, urlRegex ) ) {
            std::string domain( match[2].str() );
            std::string path( match[3].str() );

            if ( domain.find( "www." ) == 0 )
                domain = domain.substr( 4 );

            // if the path is empty, set it as "/"
            if ( path.empty() )
                path = "/";

            return std::make_pair( domain, path );
        }

        // if the URL is invalid, return nothing
        return std::make_pair( "", "" );
    }
}