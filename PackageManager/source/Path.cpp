#include "Path.h"

#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

static std::vector<std::string> split(const std::string &s, char delim) 
{
    std::vector<std::string> elems;

	std::stringstream ss(s);
    std::string item;

    while( std::getline(ss, item, delim) ) 
	{
        elems.push_back(item);
    }

    return elems;
}

Path::Path( const std::string& path )
{
	m_folders.reserve( 10 );

	auto pathCopy = path;

	transform(	pathCopy.begin(),
				pathCopy.end(),
				pathCopy.begin(),
				[]( char ch )
				{
					if( ch == '\\' )
						return '/';

					return ch;
				});


	m_folders = split( pathCopy, '/' );
}

void Path::up( unsigned levels )
{
	for(	auto i = m_folders.size() - 1; 
			levels != 0; 
			--levels, --i )
	{
		m_folders[ i ].clear();
	}
}

string Path::toString() const
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

void Path::print()
{
	for( auto i : m_folders )
	{
		cout << i << endl;
	}
}