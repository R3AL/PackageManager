#include "Profile.hpp"

Profile::Profile( const std::string& profileName ):
	m_name( profileName )
{
	load();
}

auto Profile::name() const -> std::string
{
	return m_name;
}

auto Profile::save() const -> void
{
	//TODO: save settings to PROFILE_PATH/profileName.profile
}

auto Profile::settings() const -> const Settings&
{
	return m_settings;
}

auto Profile::load( std::string profileName ) -> void
{
	if( profileName.empty() )
	{
		profileName = m_name;
	}

	//TODO: load settings from PROFILE_PATH/profileName.profile
}