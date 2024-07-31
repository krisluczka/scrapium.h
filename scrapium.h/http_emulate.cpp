#include "scrapium.h"
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

namespace scrapium {

    std::string http_emulate( const std::string& url ) {
        // session opening
        HINTERNET hInternet = InternetOpenA( "Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );
        if ( !hInternet ) {
            std::cerr << "The session wasn't started correctly." << std::endl;
            return "";
        }

        // opening given URL in a session
        HINTERNET hUrl = InternetOpenUrlA( hInternet, url.c_str(), "Accept: text/html", 0, INTERNET_FLAG_RELOAD, 0 );
        if ( !hUrl ) {
            std::cerr << "Invalid URL." << std::endl;
            InternetCloseHandle( hInternet );
            return "";
        }

        // the content
        std::string content( "" );
        // the buffer
        constexpr DWORD bufferSize( 1024 );
        char buffer[bufferSize];
        DWORD bytesRead( 0 );

        // reading (idk if it is truly optimized)
        while ( InternetReadFile( hUrl, buffer, bufferSize - 1, &bytesRead ) && bytesRead > 0 ) {
            buffer[bytesRead] = '\0'; // null terminator
            content.append( buffer, bytesRead );
        }

        // don't forget this!
        InternetCloseHandle( hUrl );
        InternetCloseHandle( hInternet );

        return content;
    }
}
