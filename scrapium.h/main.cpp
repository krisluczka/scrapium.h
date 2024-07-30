#include <iostream>
#include "scrapium.h"

int main() {
    scrapium::contents content = scrapium::scrape( "https://www.onet.pl/", "a" );

    content.print( scrapium::print_type::JSON );
}
