#include <iostream>
#include "scrapium.h"

int main() {
    scrapium::contents content = scrapium::scrap( "example.com", "/", "<a", "a>");

    content.print( scrapium::print_type::JSON );
}
