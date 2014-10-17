#include "Library.hpp"

#include <initializer_list>
#include <vector>
#include <thread>
#include <atomic>
#include <numeric>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "utils/Strings.hpp"
#include "utils/FormattedPrint.hpp"
#include "utils/FileDownloader.hpp"
#include "utils/ProgressIndicator.hpp"

#include "SettingsManager.hpp"
#include "Script.hpp"

const std::string Library::extensions[] =
{
	"rar",
	"zip",
	"gz",
	"bz2",
	"z",
	"xz"
};

Library::Library(const std::string& name, const std::string& url, const std::string& scriptLocation):
	m_name				( name ),
	m_url				( url ),
	m_scriptLocation	( scriptLocation ),
	m_valid				( true ),
	m_type				( FileType::Unknown ),
	m_archtype			( ArchiveType::Unknown )
{
	if( m_name.empty() )
	{
		m_valid = false;
		return;
	}

	if( m_url.empty() )
	{
		m_valid = false;
		return;
	}

	identifyType();
}

auto Library::name() const -> std::string
{
	return m_name;
}

auto Library::url() const -> std::string
{
	return m_url;
}

auto Library::scriptLocation() const -> std::string
{
	return m_scriptLocation;
}

auto Library::type() const -> FileType
{
	return m_type;
}

auto Library::archType() const -> ArchiveType
{
	return m_archtype;
}

auto Library::isValid() const -> bool
{
	return m_valid;
}

auto Library::identifyType() -> void
{
	using namespace utils;

	auto lastDotPos = m_url.find_last_of('.');

	if( lastDotPos != std::string::npos )
	{
		auto lastExtension		= m_url.substr( lastDotPos + 1 );
		auto archiveTypeIndex	= lastDotPos + 1;

		auto it = std::find(std::begin(extensions),
							std::end(extensions),
							Strings::Keep(lastExtension).tolower().str() );

		if( it != std::end( extensions ) )
		{
			const auto endIt	= m_url.begin() + lastDotPos - 1;

			auto secondDotPos	= std::find(m_url.begin(),
											endIt,
											'.');

			archiveTypeIndex = std::distance(	std::begin(extensions),
												it );

			lastExtension		= m_url.substr( archiveTypeIndex );
			m_type				= FileType::Archive;
			m_archtype			= archiveTypeByIndex( archiveTypeIndex );
		}
		else
		{
			if( lastExtension == "h" )
			{
				m_type		= FileType::SingleHeaderH;
				m_archtype	= ArchiveType::Unknown;
			}
			else if( lastExtension == "hpp" )
			{
				m_type		= FileType::SingleHeaderHPP;
				m_archtype	= ArchiveType::Unknown;
			}
			else
			{
				FormattedPrint::On(std::cout)	.app("Invalid extension found [")
												.color( Yellow )
												.app( lastExtension )
												.color()
												.app("]")
												.endl();

				m_valid = false;
			}
		}
	}
	else
	{
		FormattedPrint::On(std::cout)	.app("No valid extension found for library [")
										.color( Yellow )
										.app( name() )
										.color()
										.app("]")
										.endl();

		m_valid = false;
	}
}

auto Library::archiveTypeByIndex(const std::size_t& index ) const -> ArchiveType
{
	return ArchiveType( index );
}

auto Library::extensionStringFromUrl()	const -> std::string
{
	if( m_archtype != ArchiveType::Unknown )
	{
		std::string extenstion;

		if( m_archtype > ArchiveType::ZIP )
		{
			extenstion = "tar.";
		}

		return extenstion + extensions[ static_cast<std::size_t>( m_archtype ) ];
	}
	else
	{
		switch( m_type )
		{
			case FileType::SingleHeaderH:	return "h";
			case FileType::SingleHeaderHPP: return "hpp";
	
			default:
				break;
		}
	}

	return "";
}

auto Library::download() const -> bool
{
	using namespace utils;

	FormattedPrint::On(std::cout)	.app("Downloading ")
									.color( White )
									.app( name() )
									.color();

	bool downloadDone = ProgressIndicator::Task([&]
												{
													return FileDownloader::download(url(), 
																					"temp." + extensionStringFromUrl() );
												} );

	if( downloadDone )
	{
		FormattedPrint::On(std::cout, false).color( Green )
											.app("Done")
											.color()
											.endl();
	}
	else
	{
		FormattedPrint::On(std::cout, false).color( Red )
											.app("Failed")
											.color()
											.endl();
	}

	return downloadDone;
}

auto Library::extract() const -> void
{
	using namespace utils;

	if( ! isArchive() )
	{
		return;
	}

	FormattedPrint::On(std::cout)	.app("Extracting ")
									.color( White )
									.app( name() )
									.color();

	ProgressIndicator::Task([this]
							{
								static const std::string commandTemplate = "external\\bin\\7za x ";

								/*
									Extracting the archive
								*/
								std::string command = commandTemplate + "temp." + extensionStringFromUrl() + " > nul";

								system( command.c_str() ); 

								if( isTar() )
								{
									#if 1
										command = commandTemplate + "temp.tar > nul";
										system( command.c_str() ); 
									#endif
								}
							} );
	
	FormattedPrint::On(std::cout, false).color( Green )
										.app("Done")
										.color()
										.endl();
}

auto Library::clean() const	-> void
{
	using namespace utils;

	if( ! isArchive() )
	{
		return;
	}
	
	FormattedPrint::On(std::cout)	.app("Cleaning ")
									.color( White )
									.app( name() )
									.color();

	ProgressIndicator::Task([this]
							{
								auto command = "del temp." + extensionStringFromUrl() + " > nul";
								system( command.c_str() ); 

								if( isTar() )
								{
									command = "del temp.tar > nul";
									system( command.c_str() ); 
								}
							});

	FormattedPrint::On(std::cout, false).color( Green )
										.app("Done")
										.app(' ')
										.color()
										.endl();
}

auto Library::install()	const -> bool
{
	using namespace utils;

	if( download() )
	{
		extract();

		FormattedPrint::On(std::cout)	.app("Installing ")
										.color( White )
										.app( name() )
										.app(' ')
										.color()
										.endl();

		switch ( type() )
		{
			case FileType::Archive:
			{
				Script(this).run();
			}break;

			case FileType::SingleHeaderH:
				break;

			case FileType::SingleHeaderHPP:
				break;

			default:
				break;
		}

		/*
		ProgressIndicator::Task(
			[this]
			{
				// TODO: actual install
			});
		*/

		/*
		FormattedPrint::On(std::cout, false).color( Green )
											.app("Done")
											.color()
											.endl();
		*/

		//clean();

		//const auto& profile = SettingsManager::instance().activeProfile();

		return true;
	}
	else
	{
		return false;
	}
}

auto Library::isArchive() const -> bool
{
	return m_type == FileType::Archive;
}

auto Library::isTar() const -> bool
{
	return	m_archtype >= ArchiveType::TAR_GZ &&
			m_archtype <= ArchiveType::TAR_XZ;
}