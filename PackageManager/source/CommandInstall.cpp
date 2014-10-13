#include "CommandInstall.hpp"

#include <iostream>

#include "utils/FormattedPrint.hpp"
#include "utils/FileDownloader.hpp"

#include "SettingsManager.hpp"
#include "LibraryManager.hpp"

#include "CommandFactory.hpp"

const std::string CommandInstall::command = "install";

CommandInstall::CommandInstall()
{

}

bool CommandInstall::run( const std::vector< std::string >& programArguments ) const
{
	using namespace utils;

	if( programArguments.empty() )
	{
		FormattedPrint::On(std::cout)	.app("Call to [")
										.color( Color::Yellow )
										.app( command )
										.color()
										.app("] without arguments is invalid")
										.endl();
		return false;
	}

	std::vector< std::string > notInstalledLibs;
	std::vector< Library > libsToInstall;

	for( const auto& argumentLib : programArguments )
	{
		auto results = LibraryManager::instance().libraries( argumentLib );

		if( results.empty() )
		{
			notInstalledLibs.push_back( argumentLib );

			FormattedPrint::On(std::cout)	.app( "Could not find lib [" )
											.color( Red )
											.app( argumentLib )
											.color()
											.app( "]" )
											.endl();

			continue;
		}

		if( results.size() != 1 )
		{
			notInstalledLibs.push_back( argumentLib );

			FormattedPrint::On(std::cout)	.app("Search for ")
											.color( Yellow )
											.app( argumentLib )
											.color()
											.app(" returned multiple results:")
											.endl();
				
			for( const auto& lib : results )
			{
				FormattedPrint::On(std::cout)	.app("\t")
												.color( Green )
												.app( lib.name() )
												.color()
												.endl();

				continue;
			}
		}
		else
		{
			libsToInstall.push_back( results[0] );
		}
	}

	if( ! notInstalledLibs.empty() )
	{
		FormattedPrint::On(std::cout)	.app("The follow libraries were NOT installed:")
										.endl();

		for( const auto& lib : notInstalledLibs )
		{
			FormattedPrint::On(std::cout)	.app("\t")
											.color( Red )
											.app( lib )
											.color()
											.endl();
		}
	}

	// TODO: actually install the damn libs

	FormattedPrint::On(std::cout)	.app("The follow libraries were installed:")
									.endl();

	for( auto& lib : libsToInstall )
	{
		FormattedPrint::On(std::cout)	.app("\t")
										.color( Green )
										.app( lib.name() )
										.color()
										.endl();

		lib.install();
	}

	return true;
}

REGISTER_COMMAND( CommandInstall );