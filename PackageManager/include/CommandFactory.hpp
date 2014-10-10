#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

class ICommand;

class CommandFactory
{
	static auto getCommandHandlers() -> std::unordered_map<	std::string, 
															std::function<ICommand*()> >&;

public:
	template <typename T>
	static auto registerCommand() -> bool
	{
		CommandFactory::getCommandHandlers()[ T::command ] = 
			[]
			{
				return new T();
			};

		return false;
	}

	static auto get(const std::string& command) -> ICommand*;
};

#ifndef REGISTER_COMMAND
	#define REGISTER_COMMAND(className) static auto DUMMY__VAR = CommandFactory::registerCommand<className>()
#endif