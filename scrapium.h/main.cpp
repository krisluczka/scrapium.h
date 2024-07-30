#include <iostream>
#include "scrapium.h"

int main() {
    scrapium::contents content = scrapium::scrape( "https://www.example.com/", "p" );

    content.print( scrapium::print_type::PHP );
}
