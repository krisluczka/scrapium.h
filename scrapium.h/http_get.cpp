#include "scrapium.h"

#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

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
    std::string http_get( const std::string& host, const std::string& path ) {
        WSADATA wsaData;
        SOCKET ConnectSocket( INVALID_SOCKET );
        struct addrinfo* result( NULL ), * ptr( NULL ), hints;
        std::string response;

        int iResult;
        int recvbuflen( 512 );
        char recvbuf[512];

        // initializing winsock
        iResult = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
        if ( iResult != 0 ) {
            std::cerr << "WSAStartup failed: " << iResult << std::endl;
            return "";
        }

        // resolving the server and address port
        ZeroMemory( &hints, sizeof( hints ) );
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        iResult = getaddrinfo( host.c_str(), "80", &hints, &result );
        if ( iResult != 0 ) {
            std::cerr << "getaddrinfo failed: " << iResult << std::endl;
            WSACleanup();
            return "";
        }

        // creating a socket
        ConnectSocket = socket( result->ai_family, result->ai_socktype, result->ai_protocol );
        if ( ConnectSocket == INVALID_SOCKET ) {
            std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
            freeaddrinfo( result );
            WSACleanup();
            return "";
        }

        // connecting to a server
        iResult = connect( ConnectSocket, result->ai_addr, (int)result->ai_addrlen );
        if ( iResult == SOCKET_ERROR ) {
            closesocket( ConnectSocket );
            ConnectSocket = INVALID_SOCKET;
        }

        freeaddrinfo( result );

        if ( ConnectSocket == INVALID_SOCKET ) {
            std::cerr << "Unable to connect to server!" << std::endl;
            WSACleanup();
            return "";
        }

        // HTTP get
        std::string request = "GET " + path + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
        iResult = send( ConnectSocket, request.c_str(), (int)request.length(), 0 );
        if ( iResult == SOCKET_ERROR ) {
            std::cerr << "send failed: " << WSAGetLastError() << std::endl;
            closesocket( ConnectSocket );
            WSACleanup();
            return "";
        }

        // receiving
        response.clear();
        do {
            iResult = recv( ConnectSocket, recvbuf, recvbuflen, 0 );
            if ( iResult > 0 ) {
                response.append( recvbuf, iResult );
            } else if ( iResult == 0 ) {
                break;
            } else {
                std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
                closesocket( ConnectSocket );
                WSACleanup();
                return "";
            }
        } while ( iResult > 0 );

        // cleaning up
        closesocket( ConnectSocket );
        WSACleanup();

        return response;
    }
#elif defined(__linux__)
    std::string http_get( const std::string& host, const std::string& path ) {
        int sockfd;
        struct addrinfo hints, * res;
        std::string response;
        int status;
        char recvbuf[4096];
        ssize_t bytes_received;
        const int recvbuflen = sizeof( recvbuf );

        // resolving the server and address port
        memset( &hints, 0, sizeof hints );
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if ( (status = getaddrinfo( host.c_str(), "80", &hints, &res )) != 0 ) {
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
        std::string request = "GET " + path + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
        if ( send( sockfd, request.c_str(), request.length(), 0 ) == -1 ) {
            std::cerr << "send failed: " << strerror( errno ) << std::endl;
            close( sockfd );
            return "";
        }

        // receiving
        response.clear();
        while ( (bytes_received = recv( sockfd, recvbuf, recvbuflen - 1, 0 )) > 0 ) {
            recvbuf[bytes_received] = '\0';
            response.append( recvbuf, bytes_received );
        }

        if ( bytes_received == -1 ) {
            std::cerr << "recv failed: " << strerror( errno ) << std::endl;
        }

        // cleaning up
        close( sockfd );

        return response;
    }
#else
    std::string http_get( const std::string& host, const std::string& path ) {
        return "Unsupported OS";
    }
#endif
}
