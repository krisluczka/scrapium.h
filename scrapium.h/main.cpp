#include <iostream>
#include "scrapium.h"

int main() {
    std::string host = "example.com";
    std::string path = "/";
    std::string response = getHTTPResponse( host, path );

    std::cout << response;

    return 0;
}
