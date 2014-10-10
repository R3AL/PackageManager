#include "Path.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "utils/Strings.hpp"

using namespace std;


Path::Path()
{

}

Path::Path( const std::string& path )
{
	m_folders.reserve( 10 );

	m_folders =	utils::Strings::Keep( path ).replace( '\\', '/' ).split( '/' );
}

void Path::up( unsigned levels )
{
	std::size_t index;

	for(index = m_folders.size() - 1; 
		levels != 0; 
		--levels, --index )
	{
		m_folders[ index ].clear();
	}

	m_folders.resize( index + 1 );
}

Path::operator string() const
{
	string path;

	for( const auto& folder : m_folders )
	{
		if( ! folder.empty() )
		{
			if( ! path.empty() )
			{
				path += '/';
			}

			path += folder;
		}
	}

	return path;
}

void Path::cd( const std::string& folder )
{
	m_folders.push_back( folder );
}

bool Path::isFolder() const
{
	return true;
}

bool Path::isFile() const
{
	return false;
}

bool Path::isEmpty() const
{
	return m_folders.empty();
}