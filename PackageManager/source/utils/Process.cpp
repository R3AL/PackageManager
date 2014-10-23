#include "utils/Process.hpp"

#include "null.hpp"

#include <numeric>

Process::Process( const std::string& process ):
	m_process( process )
{

}

auto Process::arg( const std::string& arg ) const -> const Process&
{
	m_arguments.push_back( arg );

	return *this;
}

auto Process::arg( const char* arg ) const -> const Process&
{
	m_arguments.push_back( arg );

	return *this;
}

auto Process::redirect( const Channel& channel, 
						const std::string& to, 
						const RedirectionType& type ) const -> const Process&
{
	auto& redirectChannel = m_redirects[ channel ];

	std::get<0>( redirectChannel ) = to;
	std::get<1>( redirectChannel ) = type == Append ? true : false;
	std::get<2>( redirectChannel ) = false;

	return *this;	
}

auto Process::redirect( const Channel& channel, 
						const Channel& to ) const -> const Process&
{
	auto& redirectChannel = m_redirects[ channel ];

	if( to != Null )
	{
		std::get<0>( redirectChannel ) = std::to_string( static_cast<int>(to) );
	}
	else
	{
		#ifdef _WIN32
			std::get<0>( redirectChannel ) = "nul";
		#else
			std::get<0>( redirectChannel ) = "/dev/null";
		#endif
	}

	std::get<1>( redirectChannel ) = false;
	std::get<2>( redirectChannel ) = true;

	return *this;
}

#ifdef _WIN32
	#define popen(a, b) _popen( (a), (b) )
	#define pclose(a)	_pclose((a))
#else
	#define popen(a, b) popen( (a), (b) )
	#define pclose(a)	pclose((a))
#endif

auto Process::run() const -> int
{
	auto command = m_process;

	command += std::accumulate(	m_arguments.begin(),
								m_arguments.end(),
								std::string(),
								[]( const std::string& str, const std::string& param )
								{
									return str + ' ' + param;
								});

	if( ! m_redirects.empty() )
	{
		for( const auto& redirect : m_redirects )
		{
			command += ' ';

			const auto& channel		= redirect.first;
			const auto& destination = std::get<0>( redirect.second );
			const auto& append		= std::get<1>( redirect.second );
			const auto& isChannel	= std::get<2>( redirect.second );

			if( channel == StdError )
			{
				command += std::to_string( static_cast<int>( channel ) );
			}

			if( append )
			{
				command += ">>";
			}
			else
			{
				command += '>';

				if( isChannel )
				{
					command += '&';
				}
			}

			command += ' ' + destination;
		}
	}

	m_arguments.clear();
	m_redirects.clear();

	auto proc = popen( command.c_str(), "r" );

	if( proc == null )
	{
		throw std::invalid_argument( "Invalid command: " + command );
	}

	return pclose(proc);
}

#undef popen
#undef pclose

const Process Process::Copy = 
	#ifdef _WIN32
		/*	
		*	/Y - don't prompt for overwrite
		*/
		Process("copy", "/Y");
	#else
		Process("cp", "-r");
	#endif

const Process Process::CopyFolder = 
	#ifdef _WIN32
		/*
		*	/E - copies directories and subdirectories, including empty ones
		*	/I - assume destination is a folder if it does not exist
		*	/Y - don't prompt for overwrite
		*/
		Process("xcopy", "/E", "/I", "/Y");
	#else
		Process::Copy;
	#endif

const Process Process::Remove = 
	#ifdef _WIN32
		Process("del");
	#else
		Process("rm", "-rf");
	#endif