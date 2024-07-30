#include <iostream>
#include "scrapium.h"

int main() {
    std::cout << scrapium::http_get( "wierszespodtaboreta.pl", "/" );
    scrapium::contents content = scrapium::scrape( "https://www.google.com/", "p" );

    content.print( scrapium::print_type::YAML );
}
