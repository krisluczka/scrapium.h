#include "scrapium.h"

namespace scrapium {
    contents scrap( const std::string& host, const std::string& path, const std::string& startChar, const std::string& endChar ) {
        size_t start_pos( 0 );
        size_t end_pos( 0 );
        contents result;
        std::string token;

        // downloading the content using GET
        const std::string content( http_get( host, path ) );

        while ( true ) {
            // searching for the starting character
            start_pos = content.find( startChar, end_pos );
            if ( start_pos == std::string::npos ) break;

            // moving past the starting character
            ++start_pos;

            // finding the ending character after the starting one
            end_pos = content.find( endChar, start_pos );
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
}