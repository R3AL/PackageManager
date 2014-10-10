#include "CommandHelp.hpp"

#include <iostream>

const std::string CommandHelp::command = "--help";

CommandHelp::CommandHelp()
{

}

bool CommandHelp::run( const std::vector< std::string >& programArguments ) const
{
	std::cout << "HELP" << std::endl;

	return true;
}

REGISTER_COMMAND( CommandHelp );