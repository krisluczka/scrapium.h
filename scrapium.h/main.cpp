#include <iostream>
#include "scrapium.h"

int main() {
    scrapium::contents content = scrapium::scrape( "https://www.google.com/", "a" );

    content.print( scrapium::print_type::JSON );
}
