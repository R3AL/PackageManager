#include "Path.hpp"
#include "Convert.hpp"
#include "utils/Strings.hpp"
#include "CommandDispatcher.hpp"

#include <iostream>

int main( int argc, char** argv )
{
	CommandDispatcher dispatcher( argc, argv );

	if( dispatcher.hadErrors() )
	{
		for( const auto& error : dispatcher.getErrors() )
		{
			std::cout << error << "\n";
		}
	}
	

	/*
	using namespace utils;

	std::string str = "Testing string replacer";

	std::cout <<	Strings::Mutate(str).replace(' ', '-')
										.replace("string","NOT A STRING")
										.toupper() << "\n";

	std::cout <<	Strings::Keep("Another test of string replacer").replace(' ', '=')
																	.replace("test","NOT A TEST")
																	.tolower() << "\n";
	*/

	//FileDownloader::download( "https://github.com/R3AL/test/raw/master/node/node.exe", "node.exe" );

	std::cin.get();	
}
