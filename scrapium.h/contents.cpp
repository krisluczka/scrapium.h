#include "scrapium.h"

namespace scrapium {
	void contents::print( print_type type, const std::string& path ){
		std::ofstream file( path );

		if ( type == RAW ) {
			for ( std::string s : contents )
				if ( path == "" ) std::cout << s << "\n";
				else			  file << s << "\n";
		} else if ( type == JSON ) {
			std::string result;
			uint_fast64_t i( 0 );

			if ( path == "" ) std::cout << "{\n";
			else			  file << "{\n";
			
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

				if ( path == "" ) std::cout << "\t\"" << i << "\": \"" << result << "\",\n";
				else			  file << "\t\"" << i << "\": \"" << result << "\",\n";
				++i;
			}

			if ( path == "" ) std::cout << "}";
			else			  file << "}";
		} else if ( type == XML ) {
			std::string result;

			if ( path == "" ) std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<contents>\n";
			else			  file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<contents>\n";

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

				if ( path == "" ) std::cout << "\t<element>" << result << "</element>\n";
				else			  file << "\t<element>" << result << "</element>\n";

			}

			if ( path == "" ) std::cout << "</contents>";
			else			  file << "</contents>";
		} else if ( type == PHP ) {
			std::string result;
			uint_fast64_t i( 0 );

			if ( path == "" ) std::cout << "a:" << contents.size() << ":{";
			else			  file << "a:" << contents.size() << ":{";

			for ( std::string s : contents ) {
				result = "";
				for ( char c : s ) {
					switch ( c ) {
						case '\"':
							result += "\\\"";
							break;
						case '\'':
							result += "\\\'";
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

				if ( path == "" ) std::cout << "i:" << i << ";s:" << result.size() << ":\"" << result << "\";";
				else			  file << "i:" << i << ";s:" << result.size() << ":\"" << result << "\";";
				++i;
			}

			if ( path == "" ) std::cout << "}";
			else			  file << "}";
		} else if ( type == YAML ) {
			if ( path == "" ) std::cout << "contents:\n";
			else			  file << "contents:\n";
			
			for ( std::string s : contents )
				if ( path == "" ) std::cout << " - \'" << s << "\'\n";
				else			  file << " - \'" << s << "\'\n";
		}

		file.close();
	}
}