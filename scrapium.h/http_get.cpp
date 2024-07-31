#include "scrapium.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

namespace scrapium {
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

                size_t location_pos( response.find( "Location: " ) );
                if ( location_pos != std::string::npos ) {
                    size_t location_end = response.find( "\r\n", location_pos );
                    if ( location_end != std::string::npos ) {
                        std::string location_header( response.substr( location_pos, location_end - location_pos ) );
                        std::string location_url( location_header.substr( 10 ) );
                        return http_emulate( location_url );
                    }
                }
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
}