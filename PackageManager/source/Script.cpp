#include "Script.hpp"

#include <fstream>
#include <sstream>
#include <numeric>

#include "utils/FormattedPrint.hpp"
#include "utils/Strings.hpp"
#include "utils/Switch.hpp"
#include "utils/Process.hpp"

#include "Library.hpp"
#include "Settings.hpp"
#include "SettingsManager.hpp"

Script::Script( const Library* const library ):
	m_library( library ),
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

	if( scriptSections.empty() )
	{
		FormattedPrint::On(std::cout)	.app( "Library [")
										.color( Yellow )
										.app( m_library->name() )
										.color()
										.app("] has an invalid install.script !")
										.endl();

		m_valid = false;
		return;
	}

	unsigned sectionIndex = 0;
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
			sectionIndex++;
			continue;
		}

		const auto& sectionName	= sectionElements[0];

		auto section = Switch<Section>(sectionName)	.Case("compilers",		Section::Compilers)
													.Case("preinstall",		Section::PreInstall)
													.Case("install",		Section::Install)
													.Case("postinstall",	Section::PostInstall)
													.Case("remove",			Section::Remove)
													.Default(				Section::Unknown)
													.eval();

		if( section == Section::Compilers )
		{
			auto contentElements = Strings::Keep(sectionContent).split(',');

			std::unordered_map< Compiler, 
								Version > supportedCompilers;

			for( const auto& element : contentElements )
			{
				auto keyValuePairVec	= Strings::Keep(element).split(':');
				const auto& key			= keyValuePairVec[0];

				if( keyValuePairVec.size() != 2 )
				{
					FormattedPrint::On(std::cout)	.app( "Library [")
													.color( Yellow )
													.app( m_library->name() )
													.color()
													.app("] key [")
													.color( Yellow )
													.app( key )
													.color()
													.app("] does not have a value !")
													.endl();

					m_valid = false;
					return;
				}

				auto compiler = Switch<Compiler>(key)	.Case("MSVC",	Compiler::MSVC)
														.Case("CLANG",	Compiler::CLANG)
														.Case("GCC",	Compiler::GCC)
														.Case("ICC",	Compiler::ICC)
														.Default(		Compiler::Unknown )
														.eval();
				
				if( compiler == Compiler::Unknown )
				{
					FormattedPrint::On(std::cout)	.app( "Library [")
													.color( Yellow )
													.app( m_library->name() )
													.color()
													.app("] Invalid compiler key !")
													.endl();

					m_valid = false;
					return;
				}

				const auto& value = keyValuePairVec[1];

				supportedCompilers[ compiler ] = Version( value );
			}

			const auto& ActiveProfileSettings = SettingsManager::instance().activeProfile().settings();
			
			if( (	supportedCompilers.count( ActiveProfileSettings.compiler ) == 0 ) ||
					ActiveProfileSettings.compilerVersion < supportedCompilers[ ActiveProfileSettings.compiler ])
			{
				FormattedPrint::On(std::cout)	.app( "The current profile compiler [")
												.color( Yellow )
												.app( ActiveProfileSettings.compiler )
												.color()
												.app("] does not support this library !")
												.endl();

				m_valid = false;
				return;
			}


		}
		else if( section == Section::Unknown )
		{
			FormattedPrint::On(std::cout)	.app( "Library [")
											.color( Yellow )
											.app( m_library->name() )
											.color()
											.app("] found unknown section [")
											.app( sectionName )
											.app("] !")
											.endl();
		}
		else
		{
			if( sectionIndex == 0 )
			{
				FormattedPrint::On(std::cout)	.app( "Library [")
												.color( Yellow )
												.app( m_library->name() )
												.color()
												.app("] compilers section must be first !")
												.endl();

				m_valid = false;
				return;
			}

			auto& scriptSectionEntry	= m_sections[ section ];
			auto contentElements		= Strings::Keep(sectionContent).split(',');

			for( const auto& element : contentElements )
			{
				auto keyValuePairVec	= Strings::Keep(element).split(':');
				const auto& key			= keyValuePairVec[0];

				if( keyValuePairVec.size() != 2 )
				{
					FormattedPrint::On(std::cout)	.app( "Library [")
													.color( Yellow )
													.app( m_library->name() )
													.color()
													.app("] key [")
													.color( Yellow )
													.app( key )
													.color()
													.app("] does not have a value !")
													.endl();

					m_valid = false;
					return;
				}

				auto value = keyValuePairVec[1];

				if( value[0] == ' ' )
				{
					Strings::Mutate(value).trimLeft(1);
				}

				scriptSectionEntry[ key ] = value;
			}
		}

		sectionIndex++;
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

auto Script::runCommand(const std::string& command,
						const std::string& argument) const -> void
{
	auto initList = { argument };

	return runCommand( command, initList );
}

auto Script::runCommand(const std::string& command,
						std::initializer_list<std::string> arguments) const -> void
{
	return runCommand(	command, 
						std::vector<std::string>(	arguments.begin(), 
													arguments.end() ) );
}

auto Script::runCommand(const std::string& command,
						const std::vector<std::string>& arguments) const -> void
{
	using namespace utils;

	auto scriptCommand = Switch<ScriptCommand>(command)	.Case("print",	ScriptCommand::Print)
														.Case("run",	ScriptCommand::Run)
														.Case("copy",	ScriptCommand::Copy)
														.Case("delete", ScriptCommand::Delete)
														.Default(		ScriptCommand::Unknown)
														.eval();

	switch( scriptCommand )
	{
		case ScriptCommand::Print:
		{
			FormattedPrint::On(std::cout)	.color( Purple )
											.app( "Script: " )
											.color()
											.app( arguments[0] )
											.endl();
		}break;

		case ScriptCommand::Run:
		{
			Process(arguments[0])	.arg( std::accumulate(	arguments.begin() + 1,
															arguments.end(),
															std::string() ) )
									.redirect(	StdOutput, 
												"logs/" + m_library->name() + "_install.script.log", 
												Append)
									.redirect(	StdError, 
												StdOutput)
									.run();
		}break;

		case ScriptCommand::Copy:
		{
			Process::Copy	.arg(arguments[0])
							.redirect(	StdOutput, 
										"logs/" + m_library->name() + "_install.script.log", 
										Append)
							.redirect(	StdError, 
										StdOutput)
							.run();
		}break;

		case ScriptCommand::CopyFolder:
		{
			Process::CopyFolder	.arg(arguments[0])
								.redirect(	StdOutput, 
											"logs/" + m_library->name() + "_install.script.log", 
											Append)
								.redirect(	StdError, 
											StdOutput)
								.run();
		}break;

		case ScriptCommand::Unknown:
		{
			FormattedPrint::On(std::cout)	.app( "Unknown command [")
											.color( Yellow )
											.app( command )
											.color()
											.app("] in install.script" )
											.endl();
		}break;
	}
}

auto Script::run() const -> void
{
	if( m_valid )
	{
		const auto& Section = section( Section::PreInstall );

		for( const auto& commandValuePair : Section )
		{
			const auto& command = commandValuePair.first;
			const auto& value	= commandValuePair.second;

			runCommand( command, value );
		}
	}
}