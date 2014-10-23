#include "utils/Filesystem.hpp"

namespace utils
{
	auto Filesystem::isDirectory(const std::string& fsPath) -> bool
	{
		using namespace std::tr2::sys;

		return is_directory( path(fsPath) );
	}

	auto Filesystem::isFile(const std::string& fsPath) -> bool
	{
		using namespace std::tr2::sys;

		return is_regular_file( path(fsPath) );
	}

	auto Filesystem::currentPath() -> std::string
	{
		using namespace std::tr2::sys;

		return current_path<path>().string();
	}

	auto Filesystem::changeCurrentPath(const std::string& fsPath) -> std::string
	{
		using namespace std::tr2::sys;
		
		current_path( path(fsPath) );

		return currentPath();
	}
}