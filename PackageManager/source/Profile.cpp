#include "Profile.hpp"

#include "utils/tinyxml2.h"
#include "utils/FormattedPrint.hpp"
#include "utils/Switch.hpp"

#include "SettingsManager.hpp"

const Profile& Profile::null = Profile("");

Profile::Profile( const std::string& profileName ):
	m_name( profileName )
{
	load();
}

auto Profile::name() const -> const std::string&
{
	return m_name;
}

auto Profile::name() -> std::string&
{
	return m_name;
}

auto Profile::settings() const -> const Settings&
{
	return m_settings;
}

auto Profile::settings() -> Settings&
{
	return m_settings;
}

auto Profile::save() const -> void
{
	//TODO: save settings to PROFILE_PATH/profileName.profile
}

auto Profile::load( std::string profileName ) -> void
{
	if( profileName.empty() )
	{
		profileName = m_name;
	}

	if( !profileName.empty() )
	{
		//TODO: load settings from PROFILE_PATH/profileName.profile

		const auto ActiveProfilePath = SettingsManager::instance().global().PROFILES_FOLDER_NAME + '/' + profileName;

		tinyxml2::XMLDocument profileDoc;

		if( tinyxml2::XML_NO_ERROR != profileDoc.LoadFile( ActiveProfilePath.c_str() ) )
		{
			utils::FormattedPrint::On(std::cout).app( "File open" )
												.color( utils::Red )
												.app( " failed" )
												.color()
												.app(" [")
												.app( profileName )
												.app(']')
												.endl();
			return;
		}

		auto profileNode = profileDoc.FirstChildElement( "profile" );

		if( ! profileNode )
		{
			utils::FormattedPrint::On(std::cout).color( utils::Red)
												.app( "Invalid" )
												.color()
												.app(' ')
												.app( profileName )
												.endl();

			return;
		}

		auto settingsNode = profileNode->FirstChildElement( "settings" );

		if( ! settingsNode )
		{
			utils::FormattedPrint::On(std::cout).color( utils::Red)
												.app( "Invalid" )
												.color()
												.app(' ')
												.app( profileName )
												.endl();

			return;
		}

		auto compilerNode = settingsNode->FirstChildElement( "compiler" );

		if( ! compilerNode )
		{
			utils::FormattedPrint::On(std::cout).color( utils::Red)
												.app( "Invalid" )
												.color()
												.app(' ')
												.app( profileName )
												.endl();

			return;
		}

		std::string compilerName	= compilerNode->Attribute( "name" );
		std::string compilerVersion = compilerNode->Attribute( "version" );
		std::string binPath			= compilerNode->Attribute( "binPath" );

		utils::Switch<Compiler>( compilerName )	.Case("MSVC",	Compiler::MSVC)
												.Case("GCC",	Compiler::GCC)
												.Case("CLANG",	Compiler::CLANG)
												.Case("ICC",	Compiler::ICC).eval();
	}
}