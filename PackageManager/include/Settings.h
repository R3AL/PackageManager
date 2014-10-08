#pragma once

#include <string>

class Settings
{
public:
	enum class Compiler
	{
		MSVC,
		GCC,
		CLANG
	};

	Compiler compiler;

	// complete path to the compiler binary file (cl.exe, gcc.exe etc or the equivalent on linux)
	std::string pathToBinary;
	
	// complete path to 'include' folder of the compiler
	std::string pathToIncludes;

	// complete path to 'lib' folder of the compiler
	std::string pathToLib;
};