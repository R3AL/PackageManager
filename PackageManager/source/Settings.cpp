#include "Settings.hpp"

#include "utils/Strings.hpp"
#include "utils/FormattedPrint.hpp"

const char* const Settings::Global::SETTINGS_FILE			= "settings.xml";
const char* const Settings::Global::LIBRARY_LIST_FILE		= "libs.xml";
const char* const Settings::Global::LIBRARY_LIST_URL		= "https://raw.githubusercontent.com/R3AL/PackageManager/master/Debug/libs.xml";
std::string Settings::Global::PROFILES_FOLDER_NAME			= "profiles";

Version::Version():
	major(0),
	minor(0),
	build(0)
{

}

Version::Version(	const std::size_t& major,
					const std::size_t& minor,
					const std::size_t& build ):
	major(major),
	minor(minor),
	build(build)
{

}

Version::Version(const std::string& version):
	major(0),
	minor(0),
	build(0)
{
	using namespace utils;
	using namespace std;

	auto versionElements = Strings::Keep(version).split('.');

	if( versionElements.size() < 3 )
	{
		FormattedPrint::On(std::cout)	.app( "Invalid compiler version format" )
										.endl();
		return;
	}

	major = Strings::Keep(versionElements[0]).as<size_t>();
	minor = Strings::Keep(versionElements[1]).as<size_t>();
	build = Strings::Keep(versionElements[2]).as<size_t>();
}