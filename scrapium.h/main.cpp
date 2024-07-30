#include <iostream>
#include "scrapium.h"

int main() {
    scrapium::contents content = scrapium::scrape( "wierszespodtaboreta.pl", "/", "cite" );

    content.print( scrapium::print_type::JSON );
}
