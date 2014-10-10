#pragma once

class ICommand
{
public:
	virtual bool run(const std::vector< std::string >& programArguments) const	= 0;

	inline virtual ~ICommand()	= 0
	{}
};