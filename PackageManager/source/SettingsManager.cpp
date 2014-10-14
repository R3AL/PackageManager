#include "SettingsManager.hpp"

#include "utils/tinyxml2.h"
#include "utils/FormattedPrint.hpp"

auto SettingsManager::instance() -> SettingsManager&
{
	static SettingsManager manager;

	return manager;
}

SettingsManager::SettingsManager()
{
	using namespace utils;

	tinyxml2::XMLDocument globalSettingsDoc;

	const auto SettingsFile = global().SETTINGS_FILE;

	if( tinyxml2::XML_NO_ERROR != globalSettingsDoc.LoadFile( SettingsFile ) )
	{
		FormattedPrint::On(std::cout)	.app( "File open" )
										.color( Red )
										.app( " failed" )
										.color()
										.app(" [")
										.app( SettingsFile )
										.app(']')
										.endl();
		return;
	}

	// <settings>
	auto settingsNode = globalSettingsDoc.FirstChildElement( "settings" );

	if( ! settingsNode )
	{
		invalidXMLFile( SettingsFile );
		return;
	}

	// <profilesFolder>
	auto profilesFolderNode = settingsNode->FirstChildElement("profilesFolder");

	if( ! profilesFolderNode )
	{
		invalidXMLFile( SettingsFile );
		return;
	}

	// <activeProfile>
	auto activeProfileNode = profilesFolderNode->NextSiblingElement("activeProfile");

	if( ! activeProfileNode )
	{
		invalidXMLFile( SettingsFile );
		return;
	}

	global().PROFILES_FOLDER_NAME	= profilesFolderNode->Attribute("name");
	global().activeProfile			= activeProfileNode->Attribute("name");
}

auto SettingsManager::invalidXMLFile( const std::string& file ) const -> void
{
	utils::FormattedPrint::On(std::cout).color( utils::Red)
										.app( "Invalid" )
										.color()
										.app(' ')
										.app( file )
										.endl();
}

auto SettingsManager::global() -> Settings::Global&
{
	return m_global;
}

auto SettingsManager::global() const -> const Settings::Global&
{
	return m_global;
}

auto SettingsManager::activeProfile() const	-> Profile
{
	const auto ActiveProfile = global().activeProfile + ".xml";

	return Profile( ActiveProfile );
}