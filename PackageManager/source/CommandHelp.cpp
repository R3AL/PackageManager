#include "CommandHelp.hpp"

#include <iostream>

#include "CommandFactory.hpp"

const std::string CommandHelp::command = "--help";

CommandHelp::CommandHelp()
{

}

 // TODO
bool CommandHelp::run( const std::vector< std::string >& programArguments ) const
{
	std::cout << "HELP" << std::endl;

	return true;
}

REGISTER_COMMAND( CommandHelp );