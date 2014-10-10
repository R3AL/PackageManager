#include "CommandDispatcher.hpp"
#include "CommandFactory.hpp"
#include "ICommand.hpp"

#include <memory>

CommandDispatcher::CommandDispatcher(	const int&					argc, 
										const char* const* const	argv ):
	m_returnCode( 0 )
{
	if( argc <= 1 )
	{
		m_errors.push_back( "Please provide a valid command !" );
		m_returnCode = 1;
		return;
	}

	const std::vector< std::string > programArguments( argv + 2, argv + argc );

	m_returnCode = CommandFactory::get(argv[1])->run( programArguments ) ? 0 : 1;
}

auto CommandDispatcher::returnCode() const -> int
{
	return m_returnCode;
}

auto CommandDispatcher::hadErrors()	const -> bool
{
	return ! m_errors.empty();
}

auto CommandDispatcher::getErrors()	const -> const std::vector< std::string >&
{
	return m_errors;
}