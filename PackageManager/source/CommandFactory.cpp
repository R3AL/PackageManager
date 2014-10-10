#include "CommandFactory.hpp"

auto CommandFactory::getCommandHandlers() -> std::unordered_map<std::string, 
																std::function<ICommand*()> >&
{
	static std::unordered_map<	std::string, 
								std::function<ICommand*()> > commandHandlers;

	return commandHandlers;
}

auto CommandFactory::get( const std::string& command ) -> ICommand*
{
	return getCommandHandlers()[ command ]();
}