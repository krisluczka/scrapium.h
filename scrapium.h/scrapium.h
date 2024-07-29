#pragma once
#ifndef SCRAPIUM_H
#define SCRAPIUM_H
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

std::string getHTTPResponse( const std::string& host, const std::string& path ) {
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    std::string response;

    int iResult;
    int recvbuflen = 512;
    char recvbuf[512];

    // Initialize Winsock
    iResult = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
    if ( iResult != 0 ) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return "";
    }

    // Resolve the server address and port
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

    // Create a SOCKET for connecting to server
    ConnectSocket = socket( result->ai_family, result->ai_socktype, result->ai_protocol );
    if ( ConnectSocket == INVALID_SOCKET ) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        freeaddrinfo( result );
        WSACleanup();
        return "";
    }

    // Connect to server
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

    // Send an HTTP GET request
    std::string request = "GET " + path + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
    iResult = send( ConnectSocket, request.c_str(), (int)request.length(), 0 );
    if ( iResult == SOCKET_ERROR ) {
        std::cerr << "send failed: " << WSAGetLastError() << std::endl;
        closesocket( ConnectSocket );
        WSACleanup();
        return "";
    }

    // Receive the response
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

    // Cleanup
    closesocket( ConnectSocket );
    WSACleanup();

    return response;
}

void printTextBetween( const std::string& str, char startChar, char endChar ) {
    size_t startPos = 0;
    size_t endPos = 0;

    while ( true ) {
        // Find the start character
        startPos = str.find( startChar, endPos );
        if ( startPos == std::string::npos ) break; // No more start characters

        // Move past the start character
        startPos++;

        // Find the end character after the start character
        endPos = str.find( endChar, startPos );
        if ( endPos == std::string::npos ) break; // No more end characters

        // Extract and print the substring between start and end characters
        std::string between = str.substr( startPos, endPos - startPos );
        std::cout << between << std::endl;

        // Move past the end character for the next search
        endPos++;
    }
}

#endif