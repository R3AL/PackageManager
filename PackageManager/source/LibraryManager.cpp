#include "LibraryManager.hpp"

#include <algorithm>

#include "SettingsManager.hpp"

#include "utils/Strings.hpp"
#include "utils/FormattedPrint.hpp"
#include "utils/tinyxml2.h"

auto LibraryManager::instance() -> LibraryManager&
{
	static LibraryManager manager;

	return manager;
}

LibraryManager::LibraryManager()
{
	m_libraries.reserve( 10 );

	reload();
}

auto LibraryManager::reload() -> void
{
	using namespace tinyxml2;
	using namespace utils;

	tinyxml2::XMLDocument libListDoc;

	const auto LIBFILE = SettingsManager::instance().global().LIBRARY_LIST_FILE;

	if( XML_NO_ERROR != libListDoc.LoadFile( LIBFILE ) )
	{
		FormattedPrint::On(std::cout)	.app( "File open" )
										.color( Red )
										.app( " failed" )
										.color()
										.app(" [")
										.app( LIBFILE )
										.app(']')
										.endl();
		return;
	}

	auto librariesNode = libListDoc.FirstChildElement( "libraries" );

	if( ! librariesNode )
	{
		FormattedPrint::On(std::cout)	.color( Red)
										.app( "Invalid" )
										.color()
										.app( " libs.xml (" )
										.app( "Try running [" )
										.color( Yellow )
										.app( "update" )
										.color()
										.app("] first)")
										.endl();
		return;
	}

	for(auto lib = librariesNode->FirstChildElement("lib"); 
		lib != nullptr; 
		lib = lib->NextSiblingElement("lib"))
	{
		m_libraries.emplace_back(	lib->Attribute( "name" ),
									lib->Attribute( "url" ) );
	}
}

auto LibraryManager::libraries( const std::string& name ) -> std::vector< Library >
{
	std::vector< Library > matchedLibs;

	// Look for an exact match first
	std::copy_if(	m_libraries.begin(),
					m_libraries.end(),
					std::back_inserter( matchedLibs ),
					[&name]( const Library& lib )
					{
						using namespace utils;

						return	Strings::Keep( lib.name() ).tolower() == 
								Strings::Keep( name ).tolower();
					} );

	// if not exact matches, use "fuzzy" search
	if( matchedLibs.empty() )
	{
		std::copy_if(	m_libraries.begin(),
						m_libraries.end(),
						std::back_inserter( matchedLibs ),
						[&name]( const Library& lib )
						{
							using namespace utils;

							return Strings::Keep( lib.name() ).tolower()
															.contains( Strings::Keep( name ).tolower() );
						} );
	}

	return matchedLibs;
}