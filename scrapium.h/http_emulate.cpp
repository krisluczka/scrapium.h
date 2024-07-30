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
        int sockfd;
        struct addrinfo hints, * res, * p;
        std::string response;
        int status;
        char recvbuf[512];
        ssize_t bytes_received;
        const int recvbuflen = sizeof( recvbuf );

        // resolving the server and address port
        memset( &hints, 0, sizeof hints );
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if ( (status = getaddrinfo( url.c_str(), "80", &hints, &res )) != 0 ) {
            std::cerr << "getaddrinfo error: " << gai_strerror( status ) << std::endl;
            return "";
        }

        // creating a socket
        sockfd = socket( res->ai_family, res->ai_socktype, res->ai_protocol );
        if ( sockfd == -1 ) {
            std::cerr << "socket creation failed: " << strerror( errno ) << std::endl;
            freeaddrinfo( res );
            return "";
        }

        // connecting to a server
        if ( connect( sockfd, res->ai_addr, res->ai_addrlen ) == -1 ) {
            std::cerr << "connection failed: " << strerror( errno ) << std::endl;
            close( sockfd );
            freeaddrinfo( res );
            return "";
        }

        freeaddrinfo( res );

        // HTTP get
        std::string request = "GET " + "/" + " HTTP/1.1\r\nurl: " + url + "\r\nConnection: close\r\n\r\n";
        if ( send( sockfd, request.c_str(), request.length(), 0 ) == -1 ) {
            std::cerr << "send failed: " << strerror( errno ) << std::endl;
            close( sockfd );
            return "";
        }

        // receiving
        response.clear();
        while ( (bytes_received = recv( sockfd, recvbuf, recvbuflen, 0 )) > 0 ) {
            response.append( recvbuf, bytes_received );
        }

        if ( bytes_received == -1 ) {
            std::cerr << "recv failed: " << strerror( errno ) << std::endl;
        }

        // cleaning up
        close( sockfd );

        return response;
    }
#endif
}
