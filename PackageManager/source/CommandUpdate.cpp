#include "CommandUpdate.hpp"

#include "utils/FormattedPrint.hpp"
#include "utils/FileDownloader.hpp"
#include "utils/TaskProgressIndicator.hpp"

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

	FormattedPrint::On( std::cout )	.app("Updating library list");

	bool isSuccess = ProgressIndicator::Task(	[&]
												{ 
													return FileDownloader::download(SettingsManager::instance().global().LIBRARY_LIST_URL,
																					SettingsManager::instance().global().LIBRARY_LIST_FILE); 
												} );
	
	if( isSuccess )
	{
		FormattedPrint::On(std::cout, false).color( Green )
											.app("Done")
											.color()
											.endl();
	}
	else
	{
		FormattedPrint::On(std::cout, false).color( Red )
											.app("Failed")
											.color()
											.endl();
	}

	return isSuccess;
}

REGISTER_COMMAND( CommandUpdate );