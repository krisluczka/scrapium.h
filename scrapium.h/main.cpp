#include <iostream>
#include "scrapium.h"

int main() {
    scrapium::browser_emulation = false;

    scrapium::contents content = scrapium::scrape( "http://www.wierszespodtaboreta.pl/", "cite" );

    content.print( scrapium::print_type::JSON );
}
