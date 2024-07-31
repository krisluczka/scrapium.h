#pragma once
#ifndef SCRAPIUM_H
#define SCRAPIUM_H
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <fstream>

namespace scrapium {
	extern bool unicode_shift;

	enum print_type {
		NONE,
		RAW,
		JSON,
		XML,
		PHP,
		YAML
	};

	class contents {
		friend contents scrape( const std::string&, const std::string&, const std::string&, bool );
		friend contents scrape( const std::string&, const std::string&, bool );
		public:
			void print( print_type type = print_type::RAW, const std::string& path = "" );

		private:
			std::vector<std::string> contents;
	};

	std::string http_get( const std::string& url, const std::string& path );
	std::string http_emulate( const std::string& url );

	std::string deunicode( char32_t unicode_char );
	std::string deunicode_convert( const std::string& input );

	contents scrape( const std::string& url, const std::string& start, const std::string& end, bool fast = false );
	contents scrape( const std::string& url, const std::string& html_tag, bool fast = false );
}

#endif