#include "SettingsManager.hpp"

#include "utils/tinyxml2.h"

auto SettingsManager::instance() -> SettingsManager&
{
	static SettingsManager manager;

	return manager;
}

SettingsManager::SettingsManager()
{
		
}

auto SettingsManager::global() -> const Settings::Global&
{
	return m_global;
}

auto SettingsManager::byProfile( const std::string& profile ) -> const Settings&
{
	return m_profiles[ profile ];
}