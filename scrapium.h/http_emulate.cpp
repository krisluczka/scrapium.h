#include "scrapium.h"

#ifdef _WIN32

#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#elif defined(__linux__)
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h> 

#endif

namespace scrapium {

#ifdef _WIN32
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
#elif defined(__linux__)
    std::string http_emulate( const std::string& url ) {
        return http_get( url, "/" );
    }
#else
    std::string http_emulate( const std::string& url ) {
        return "Unsupported OS";
    }
#endif
}
