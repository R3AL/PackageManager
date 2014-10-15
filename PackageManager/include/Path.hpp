#pragma once

#include <string>
#include <vector>

#include "Convert.hpp"

class Path
{
	friend converter<Path>;

private:
	std::vector< std::string > m_folders;

public:
	operator std::string() const;

	Path();
	Path( const std::string& path );

	/*
		go up a number of folders
	*/
	auto up( unsigned levels = 1 ) -> void;

	/*
		go into a specific folder
	*/
	auto cd( const std::string& folder ) -> void;

	/*
		returns true if the rightmost element from the path is a folder
	*/
	auto isFolder() const -> bool;

	/*
		returns true if the rightmost element from the path is a file
	*/
	auto isFile() const -> bool;

	auto isEmpty() const -> bool;
};

auto operator==(const Path& p1, const Path& p2 ) -> bool;
auto operator!=(const Path& p1, const Path& p2 ) -> bool;