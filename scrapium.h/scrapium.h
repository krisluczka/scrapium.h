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
		JSON
	};

	class contents {
		friend contents scrape( const std::string&, const std::string&, const std::string&, const std::string& );
		friend contents scrape( const std::string&, const std::string&, const std::string& );
		public:
			void print( print_type type = print_type::RAW );

		private:
			std::vector<std::string> contents;
	};

	std::string http_get( const std::string&, const std::string& );

	contents scrape( const std::string& host, const std::string& path, const std::string& start, const std::string& end );
	contents scrape( const std::string& host, const std::string& path, const std::string& html_tag );
}

#endif