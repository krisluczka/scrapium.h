#include <iostream>
#include "scrapium.h"

int main() {
    scrapium::contents content = scrapium::scrape( "http://www.wierszespodtaboreta.pl/", "a" );

    content.print( scrapium::print_type::JSON );
}
