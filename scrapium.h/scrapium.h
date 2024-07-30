#pragma once
#ifndef SCRAPIUM_H
#define SCRAPIUM_H
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

namespace scrapium {
	enum print_type {
		NONE,
		RAW,
		JSON,
		XML,
		PHP,
	};

	class contents {
		friend contents scrape( const std::string&, const std::string&, const std::string&, bool );
		friend contents scrape( const std::string&, const std::string&, bool );
		public:
			void print( print_type type = print_type::RAW );

		private:
			std::vector<std::string> contents;
	};

	std::string http_get( const std::string& url, const std::string& path );
	std::string http_emulate( const std::string& url );

	contents scrape( const std::string& url, const std::string& start, const std::string& end, bool fast = false );
	contents scrape( const std::string& url, const std::string& html_tag, bool fast = false );
}

#endif