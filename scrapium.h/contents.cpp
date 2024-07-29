#include "scrapium.h"

namespace scrapium {
	void contents::print( print_type type ){
		if ( type == RAW ) {
			for ( std::string s : contents )
				std::cout << s << "\n";
		} else if ( type == JSON ) {
			uint_fast64_t i( 0 );

			std::cout << "{\n";
			for ( std::string s : contents ) {
				std::cout << "\t\"" << i << "\": \"" << s << "\",\n";
				++i;
			}
			std::cout << "}";
		}
	}
}