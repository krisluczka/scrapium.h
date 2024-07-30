#include "scrapium.h"

namespace scrapium {
	void contents::print( print_type type ){
		if ( type == RAW ) {
			for ( std::string s : contents )
				std::cout << s << "\n";
		} else if ( type == JSON ) {
			std::string result;
			uint_fast64_t i( 0 );

			std::cout << "{\n";
			for ( std::string s : contents ) {
				result = "";
				for ( char c : s ) {
					switch ( c ) {
						case '\"':
							result += "\\\"";
							break;
						case '\\':
							result += "\\\\";
							break;
						case '/':
							result += "\\/";
							break;
						case '\b':
							result += "\\b";
							break;
						case '\f':
							result += "\\f";
							break;
						case '\n':
							result += "\\n";
							break;
						case '\r':
							result += "\\r";
							break;
						case '\t':
							result += "\\t";
							break;
						default:
							result += c;
							break;
					}
				}

				std::cout << "\t\"" << i << "\": \"" << result << "\",\n";
				++i;
			}
			std::cout << "}";
		} else if ( type == XML ) {
			std::string result;

			std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
			std::cout << "<contents>\n";

			for ( std::string s : contents ) {
				result = "";
				for ( char c : s ) {
					switch ( c ) {
						case '<':
							result += "&lt;";
							break;
						case '>':
							result += "&gt;";
							break;
						case '&':
							result += "&amp;";
							break;
						case '\'':
							result += "&apos;";
							break;
						case '"':
							result += "&quot;";
							break;
						default:
							result += c;
							break;
					}
				}

				std::cout << "\t<element>" << result << "</element>\n";
			}

			std::cout << "</contents>";
		}
	}
}