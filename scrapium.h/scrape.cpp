#include "scrapium.h"

namespace scrapium {
    bool unicode_shift = false;

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

    contents scrape( const std::string& url, const std::string& start, const std::string& end, bool fast ) {
        size_t start_pos( 0 );
        size_t end_pos( 0 );
        contents result;
        std::string token;

        // downloading the content
        std::string content;
#ifdef _WIN32
        if ( fast )
            content = http_get( url, "/" );
        else
            content = http_emulate( url );
#elif defined(__linux__)
        // sorry linux
        content = http_get( url, "/" );
#else
        content = "Unsupported OS";
#endif

        // checking whether we are using unicode escapes
        if ( unicode_shift )
            content = deunicode_convert( content );

        while ( true ) {
            // searching for the starting character
            start_pos = content.find( start, end_pos );
            if ( start_pos == std::string::npos ) break;

            // moving past the starting character
            start_pos += start.size();

            // finding the ending character after the starting one
            end_pos = content.find( end, start_pos );
            if ( end_pos == std::string::npos ) break;

            // extracting the true content (removing whitespace)
            std::string true_content("");
            std::istringstream iss( content.substr( start_pos, end_pos - start_pos ) );
            while ( iss >> token )
                true_content += token + " ";
            
            result.contents.push_back( true_content );

            // moving past the ending character
            ++end_pos;
        }

        return result;
    }

    contents scrape( const std::string& url, const std::string& html_tag, bool fast ) {
        size_t start_pos( 0 );
        size_t end_pos( 0 );
        contents result;

        size_t tag_start_pos( 0 ), tag_end_pos( 0 );

        // downloading the content using GET
        std::string content, token;
#ifdef _WIN32
        if ( fast )
            content = http_get( url, "/" );
        else
            content = http_emulate( url );
#elif defined(__linux__)
        // sorry linux
        content = http_get( url, "/" );
#else
        content = "Not supported OS";
#endif

        // checking whether we are using unicode escapes
        if ( unicode_shift )
            content = deunicode_convert( content );

        std::istringstream iss( content );
        while ( iss >> token )
            content += token + " ";

        while ( true ) {
            // searching for the starting tag
            tag_start_pos = content.find( "<" + html_tag + ">", end_pos);
            if ( tag_start_pos == std::string::npos )
                tag_start_pos = content.find( "< " + html_tag + ">", end_pos );
            if ( tag_start_pos == std::string::npos )
                tag_start_pos = content.find( "<" + html_tag + " ", end_pos );
            if ( tag_start_pos == std::string::npos )
                tag_start_pos = content.find( "< " + html_tag + " ", end_pos );
            if ( tag_start_pos == std::string::npos ) break;

            tag_end_pos = content.find( ">", tag_start_pos );
            if ( tag_end_pos == std::string::npos ) break;
            start_pos = tag_end_pos + 1;

            // searching for the ending tag
            tag_start_pos = content.find( "</" + html_tag + ">", end_pos);
            if ( tag_start_pos == std::string::npos ) break;
            tag_end_pos = content.find( ">", tag_start_pos );
            if ( tag_end_pos == std::string::npos ) break;
            end_pos = tag_start_pos;

            // extracting the true content
            result.contents.push_back( content.substr( start_pos, end_pos - start_pos ) );
        
            // moving past the ending character
            ++end_pos;
        }

        return result;
    }
}