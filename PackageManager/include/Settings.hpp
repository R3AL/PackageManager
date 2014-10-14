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

struct Settings
{
	Compiler compiler;

	std::string compilerVersion;

	// complete path to the compiler binary file (cl.exe, gcc.exe etc or the equivalent on linux)
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

		Global();
	};
};