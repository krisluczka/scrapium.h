#pragma once
#ifndef SCRAPIUM_H
#define SCRAPIUM_H
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <regex>
#include <utility>
#include <fstream>

namespace scrapium {
	// Result printing types
	enum print_type {
		NONE,
		RAW,
		JSON,
		XML,
		PHP,
		YAML
	};

	// Content class to store scraped data
	class contents {
		friend contents scrape( const std::string&, const std::string&, const std::string& );
		friend contents scrape( const std::string&, const std::string& );
		public:
			void print( print_type type = print_type::RAW, const std::string& path = "" );

		private:
			std::vector<std::string> contents;
	};

	/*
		Connection
	*/
	// Uses HTTP GET protocol to download data from a site
	std::string http_get( const std::string& host, const std::string& path );
	// Emulates a browser to download data from a site
	std::string http_emulate( const std::string& url );

	/*
		Utilities
	*/
	// Unicode escape flag
	extern bool unicode_escape;
	// Browser emulation flag
	extern bool browser_emulation;
	// User agent property
	extern std::string user_agent;
	// Replaces unicode character with its \uXXXX form
	std::string deunicode( char32_t unicode_char );
	// Converts a string by swapping unicode characters with its \uXXXX form
	std::string deunicode_convert( const std::string& input );
	// Splits an URL into the domain address and its path
	std::pair<std::string, std::string> split_url( const std::string& url );

	/*
		Scraping
	*/
	// Scrapes data from given URL between every specified 'start' and 'end'
	contents scrape( const std::string& url, const std::string& start, const std::string& end );
	// Scrapes data from given URL from given HTML tags
	contents scrape( const std::string& url, const std::string& html_tag );
}

#endif