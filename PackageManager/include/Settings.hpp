#pragma once

#include <string>

#include "Path.hpp"

struct Settings
{
	enum class Compiler
	{
		MSVC,
		GCC,
		CLANG
	};

	Compiler compiler;

	// complete path to the compiler binary file (cl.exe, gcc.exe etc or the equivalent on linux)
	Path binary;
	
	// complete path to 'include' folder of the compiler
	Path includes;

	// complete path to 'lib' folder of the compiler
	Path lib;


	struct Global
	{
		static const std::string SETTINGS_FILE;
		static const std::string LIBRARY_LIST_FILE;
		static const std::string LIBRARY_LIST_URL;
		static const std::string PROFILES_FOLDER_NAME;
		

		// complete path to the profiles folder
		Path profiles;

		// complete path to the folder containing the file with a list of all supported libraries
		Path libraryList;

		Global();
	};
};