#pragma once

#include <string>
#include <vector>

class Path
{
	static const char OS_PATH_DELIMITER;
	
	std::vector< std::string > m_folders;

public:

	Path( const std::string& path );

	std::string toString() const;

	void up( unsigned levels = 1 );

	void print();
};