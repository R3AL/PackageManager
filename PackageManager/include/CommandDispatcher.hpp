#pragma once

#include <string>
#include <vector>

class CommandDispatcher
{
private:
	std::vector< std::string >		m_errors;
	int								m_returnCode;

public:
	CommandDispatcher(	const int&					argc, 
						const char* const* const	argv );

	auto returnCode()	const -> int;
	auto hadErrors()	const -> bool;
	auto getErrors()	const -> const std::vector< std::string >&;
};