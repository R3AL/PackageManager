#pragma once

#include <string>
#include <vector>

#include "Convert.hpp"

class Path
{
	friend converter<Path>;

private:
	operator std::string() const;

	std::vector< std::string > m_folders;

public:

	Path();
	Path( const std::string& path );

	/*
		go up a number of folders
	*/
	void up( unsigned levels = 1 );

	/*
		go into a specific folder
	*/
	void cd( const std::string& folder );

	/*
		returns true if the rightmost element from the path is a folder
	*/
	bool isFolder() const;

	/*
		returns true if the rightmost element from the path is a file
	*/
	bool isFile() const;

	bool isEmpty() const;
};