#include "Profile.hpp"

#include "utils/tinyxml2.h"
#include "utils/FormattedPrint.hpp"
#include "utils/Switch.hpp"

#include "SettingsManager.hpp"
#include "Convert.hpp"
#include "null.hpp"

Profile::Profile( const std::string& profileName ):
	m_name( profileName )
{
	load();
}

auto Profile::getNull() -> const Profile&
{
	static const Profile& null = Profile("");

	return null;
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

		if( profileNode == null )
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

		if( settingsNode == null )
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

		if( compilerNode == null )
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

		m_settings.compiler = utils::Switch<Compiler>( compilerName )	.Case("MSVC",	Compiler::MSVC)
																		.Case("GCC",	Compiler::GCC)
																		.Case("CLANG",	Compiler::CLANG)
																		.Case("ICC",	Compiler::ICC)
																		.eval();

		m_settings.compilerVersion	= Version( compilerVersion );
		m_settings.binary			= Path(binPath);

		if( compilerNode->Attribute("includesPath") != null )
		{
			m_settings.includes = Path( compilerNode->Attribute("includesPath") );
		}
		else
		{
			m_settings.includes = m_settings.binary;
			m_settings.includes.up();
			m_settings.includes.cd("include");
		}

		if( compilerNode->Attribute("libPath") != null )
		{
			m_settings.lib = Path( compilerNode->Attribute("libPath") );

		}
		else
		{
			m_settings.lib = m_settings.binary;
			m_settings.lib.up();
			m_settings.lib.cd("lib");
		}

		std::cout << "Compiler: " << int(m_settings.compiler) << std::endl;
		std::cout << "Compiler version: " << m_settings.compilerVersion.major << std::endl;
		std::cout << "Compiler version: " << m_settings.compilerVersion.minor << std::endl;
		std::cout << "Compiler version: " << m_settings.compilerVersion.build << std::endl;
		std::cout << "Binary path: " << convert(m_settings.binary).to<std::string>() << std::endl;
		std::cout << "Incl path: " << convert(m_settings.includes).to<std::string>() << std::endl;
		std::cout << "Lib path: " << convert(m_settings.lib).to<std::string>() << std::endl;
	}
}

auto operator==(const Profile& p1, const Profile& p2 ) -> bool
{
	return	p1.name()		== p2.name() &&
			p1.settings()	== p2.settings();
}

auto operator!=(const Profile& p1, const Profile& p2 ) -> bool
{
	return ! ( p1 == p2 );
}