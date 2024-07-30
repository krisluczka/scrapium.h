#include <iostream>
#include "scrapium.h"

int main() {
    scrapium::contents content = scrapium::scrape( "https://www.google.com/", "p" );

    content.print( scrapium::print_type::YAML );
}
