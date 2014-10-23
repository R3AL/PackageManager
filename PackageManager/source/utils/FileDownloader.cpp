#include "utils/FileDownloader.hpp"
#include "utils/FormattedPrint.hpp"

#include "utils/Process.hpp"

#include <iostream>

#ifdef _WIN32
	#include <urlmon.h>
	#pragma comment(lib, "Urlmon.lib")
#endif

auto FileDownloader::download(	const std::string& url,
								const std::string& fileName ) -> bool
{
	using namespace utils;

	#ifdef _WIN32
		auto rval = URLDownloadToFile(	nullptr,
										url.data(),
										fileName.data(),
										0,
										nullptr );

		return rval == S_OK;		
	#else
		Process("wget")	.arg("\"" + url + "\"")
						.arg("-O")
						.arg(fileName)
						.redirect(	StdOutput,
									Null)
						.redirect(	StdError,
									StdOutput)
						.run();

		return true;
	#endif
}