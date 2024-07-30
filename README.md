<img src="https://github.com/krisluczka/scrapium.h/blob/master/logo.png" height="100px"/>

# scrapium.h
The simplest C++ library to scrape data from the Web.

```cpp
#include "scrapium.h"

int main() {
    scrapium::contents content = scrapium::scrape( "https://www.example.com/", "a");

    content.print( scrapium::print_type::JSON );
}
```

One command to rule them all.