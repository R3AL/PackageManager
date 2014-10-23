#include "Path.hpp"

#include "utils/Strings.hpp"
#include "utils/Process.hpp"
#include "utils/Filesystem.hpp"

#include "CommandDispatcher.hpp"

#include <iostream>

int main( int argc, char** argv )
{

#if 0
	CommandDispatcher dispatcher( argc, argv );

	if( dispatcher.hadErrors() )
	{
		for( const auto& error : dispatcher.getErrors() )
		{
			std::cout << error << "\n";
		}
	}
#else
	const std::string path = "C:/Users/alexandru.merisanu/Documents/GitHub/PackageManager/PackageManager/source";
	/*
	std::cout	<< "Is dir: " << utils::Filesystem::isDirectory(path)
				<< "\nIs file: " << utils::Filesystem::isFile(path)
				<< std::endl;
	*/

	namespace fs = utils::Filesystem;

	auto results = fs::recursiveFind< fs::AllResults >(path, "*.cpp");

	for( const auto& i : results )
		std::cout << i << std::endl;
#endif


	// Copy folders
	/*
	Process("xcopy").arg("/E")
					.arg("/I")
					.arg("/Y")
					.arg("external")
					.arg("internal")
					.redirect(StdError, "logs/whatever.txt", Append)
					.run();
	*/

	// Copy files
	/*
	Process("copy")	.arg("/Y")
					.arg("input.xml")
					.arg("output.xml")
					.redirect(StdError, "logs/whatever.txt", Append)
					.run();
					*/
	
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
