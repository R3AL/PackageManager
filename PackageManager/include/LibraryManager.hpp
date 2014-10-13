#pragma once

#include <string>
#include <vector>

#include "Library.hpp"

class LibraryManager
{
private:
	std::vector< Library > m_libraries;

	LibraryManager();

	// not implemented
	auto operator new(size_t)	-> void*;
	auto operator delete(void*) -> void;

public:
	static auto instance() -> LibraryManager&;

	auto reload()								-> void;
	auto libraries( const std::string& name )	-> std::vector< Library >;
};