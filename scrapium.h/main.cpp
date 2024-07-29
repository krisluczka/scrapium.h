#include <iostream>
#include "scrapium.h"

int main() {
    scrapium::contents content;
    content = scrapium::scrap( "wierszespodtaboreta.pl", "/", "<a", "a>");

    content.print( scrapium::print_type::JSON );

    return 0;
}
