#include <iostream>
#include "scrapium.h"

int main() {
    scrapium::contents content = scrapium::scrape( "http://www.wierszespodtaboreta.pl/", "cite" );

    content.print( scrapium::print_type::PHP );
}
