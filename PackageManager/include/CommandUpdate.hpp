#pragma once

#include <vector>
#include <string>

#include "ICommand.hpp"

class CommandUpdate : public ICommand
{
public:
	static const std::string command;

	CommandUpdate();

	bool run( const std::vector< std::string >& programArguments ) const override;
};
