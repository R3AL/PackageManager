#include "CommandUpdate.hpp"

#include "utils/FormattedPrint.hpp"
#include "utils/FileDownloader.hpp"

#include "SettingsManager.hpp"

#include <iostream>

const std::string CommandUpdate::command = "update";

CommandUpdate::CommandUpdate()
{

}

bool CommandUpdate::run( const std::vector< std::string >& programArguments ) const
{
	using namespace utils;
	//FormattedPrint::cout("Updating library list", true);

	FormattedPrint::On( std::cout ).app("Updating library list");


	auto success = FileDownloader::download(SettingsManager::instance().global().LIBRARY_LIST_URL,
											SettingsManager::instance().global().LIBRARY_LIST_FILE);

	if( success )
	{
		FormattedPrint::cout("Update complete", true);
	}
	else
	{
		FormattedPrint::cout("Update FAILED", true);
	}

	return success;
}

REGISTER_COMMAND( CommandUpdate );