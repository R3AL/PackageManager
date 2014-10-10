#pragma once

#include "Settings.hpp"
#include "Profile.hpp"

#include <unordered_map>
#include <string>

class SettingsManager
{
	Settings::Global m_global;

	std::unordered_map< std::string, 
						Settings > m_profiles;

private:
	SettingsManager();

	// not implemented
	auto operator new(size_t)	-> void*;
	auto operator delete(void*) -> void;

public:
	static auto instance()							-> SettingsManager&;

	auto global()									-> const Settings::Global&;
	auto byProfile( const std::string& profile )	-> const Settings&;
};