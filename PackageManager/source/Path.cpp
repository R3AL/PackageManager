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

auto Path::up( unsigned levels ) -> void
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

auto Path::cd( const std::string& folder ) -> void
{
	m_folders.push_back( folder );
}

auto Path::isFolder() const -> bool
{
	return true;
}

auto Path::isFile() const -> bool
{
	return false;
}

auto Path::isEmpty() const -> bool
{
	return m_folders.empty();
}

auto operator==(const Path& p1, const Path& p2 ) -> bool
{
	return	static_cast<std::string>(p1) ==
			static_cast<std::string>(p2);
}

auto operator!=(const Path& p1, const Path& p2 ) -> bool
{
	return ! ( p1 == p2 );
}