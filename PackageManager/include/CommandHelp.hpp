#pragma once

#include <vector>
#include <string>

#include "ICommand.hpp"
#include "CommandFactory.hpp"

class CommandHelp : public ICommand
{
public:
	static const std::string command;

	CommandHelp();

	bool run( const std::vector< std::string >& programArguments ) const override;
};