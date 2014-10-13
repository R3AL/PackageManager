#include "CommandUpdate.hpp"

#include "utils/FormattedPrint.hpp"
#include "utils/FileDownloader.hpp"

#include "SettingsManager.hpp"

#include "CommandFactory.hpp"

#include <iostream>

const std::string CommandUpdate::command = "update";

CommandUpdate::CommandUpdate()
{

}

bool CommandUpdate::run( const std::vector< std::string >& programArguments ) const
{
	using namespace utils;

	FormattedPrint::On( std::cout )	.app("Updating library list")
									.endl();

	auto isSuccess = FileDownloader::download(	SettingsManager::instance().global().LIBRARY_LIST_URL,
												SettingsManager::instance().global().LIBRARY_LIST_FILE);
	
	if( isSuccess )
	{
		FormattedPrint::On(std::cout)	.app("Update")
										.color( Green )
										.app(" complete")
										.color()
										.endl();
	}
	else
	{
		FormattedPrint::On( std::cout )	.app("Update")
										.color( Red )
										.app(" failed")
										.color()
										.endl();
	}

	return isSuccess;
}

REGISTER_COMMAND( CommandUpdate );