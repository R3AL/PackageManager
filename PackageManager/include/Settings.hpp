#pragma once

#include <string>

#include "Path.hpp"

enum class Compiler
{
	MSVC,
	GCC,
	CLANG,
	ICC
};

struct Version
{
	std::size_t major;
	std::size_t minor;
	std::size_t build;

	Version();

	/*
	 *	For MSVC the versions are:
	 *	...
	 *	VS 2012 -> compiler version 17.0.0
	 *	VS 2013 -> compiler version 18.0.0
	 *	...
	 */
	Version(const std::size_t& major,
			const std::size_t& minor,
			const std::size_t& build);
	
	Version(const std::string& version);
};

auto operator==(const Version& v1, const Version& v2 ) -> bool;
auto operator!=(const Version& v1, const Version& v2 ) -> bool;

struct Settings
{
	Compiler compiler;

	Version compilerVersion;

	// complete path to the compiler binary file (cl.exe, gcc.exe etc or the equivalent on Linux)
	Path binary;
	
	// complete path to 'include' folder of the compiler
	Path includes;

	// complete path to 'lib' folder of the compiler
	Path lib;


	struct Global
	{
		static const char* const SETTINGS_FILE;
		static const char* const LIBRARY_LIST_FILE;
		static const char* const LIBRARY_LIST_URL;
		static std::string PROFILES_FOLDER_NAME;
		
		std::string activeProfile;

		// complete path to the profiles folder
		Path profiles;

		// complete path to the folder containing the file with a list of all supported libraries
		Path libraryList;
	};
};

auto operator==(const Settings& s1, const Settings& s2 ) -> bool;
auto operator!=(const Settings& s1, const Settings& s2 ) -> bool;