#include "Script.hpp"

#include <fstream>
#include <sstream>

#include "utils/FormattedPrint.hpp"
#include "utils/Strings.hpp"
#include "utils/Switch.hpp"

#include "Library.hpp"

Script::Script( const Library* const library ):
	m_valid(true)
{
	using namespace utils;

	const auto ScriptLocation = library->scriptLocation() + "/install.script";
	
	std::ifstream scriptFile( ScriptLocation );

	if( ! scriptFile )
	{
		FormattedPrint::On(std::cout)	.app("Could not find [")
										.color( Yellow )
										.app( ScriptLocation )
										.color()
										.app( ']' )
										.endl();

		m_valid = false;
		return;
	}

	std::stringstream buffer;
	buffer << scriptFile.rdbuf();

	scriptFile.close();

	const auto& fileContent = buffer.str();
	auto scriptSections		= Strings::Keep(fileContent).split('#');

	for( const auto& sec : scriptSections )
	{
		if( sec.empty() )
			continue;

		auto sectionElements = Strings::Keep(sec).split('{');

		for( auto& element : sectionElements )
		{
			Strings::Mutate(element).strip( { '\n', '\t', '}' } );
		}

		auto sectionContent	= sectionElements[1];

		if( sectionContent.empty() )
		{
			continue;
		}

		const auto& sectionName	= sectionElements[0];

		auto section = Switch<Section>(sectionName)	.Case("preinstall",		Section::PreInstall)
													.Case("install",		Section::Install)
													.Case("postinstall",	Section::PostInstall)
													.Case("remove",			Section::Remove)
													.eval();

		auto& scriptSectionEntry	= m_sections[ section ];
		auto contentElements		= Strings::Keep(sectionContent).split(',');

		for( const auto& element : contentElements )
		{
			auto keyValuePairVec	= Strings::Keep(element).split(':');

			const auto& key			= keyValuePairVec[0];
			const auto& value		= keyValuePairVec[1];

			scriptSectionEntry[ key ] = value;
		}
	}
}

auto Script::hasSection(const Section& section) const -> bool
{
	return m_sections.count( section ) != 0;
}

auto Script::section(const Section& section) const -> const SectionContent&
{
	return m_sections.at( section );
}

auto Script::section(const Section& section) -> SectionContent&
{
	return m_sections[ section ];
}

auto Script::run() const -> void
{
	if( m_valid )
	{

	}
}