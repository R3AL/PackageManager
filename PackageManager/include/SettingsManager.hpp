#pragma once

#include "Settings.hpp"
#include "Profile.hpp"

#include <unordered_map>
#include <string>

class SettingsManager
{
	Settings::Global m_global;

private:
	SettingsManager();

	// not implemented
	auto operator new(size_t)	-> void*;
	auto operator delete(void*) -> void;

	auto invalidXMLFile( const std::string& file ) const -> void;

public:
	static auto instance()			-> SettingsManager&;

	auto global()					->			Settings::Global&;
	auto global()			const 	-> const	Settings::Global&;

	auto activeProfile()	const	-> Profile;
};