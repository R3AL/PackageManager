#pragma once

#include <vector>
#include <string>

#include "ICommand.hpp"

class CommandInstall : public ICommand
{
public:
	static const std::string command;

	CommandInstall();

	bool run( const std::vector< std::string >& programArguments ) const override;
};
