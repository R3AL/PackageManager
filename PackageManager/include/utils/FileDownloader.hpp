#pragma once

#include <string>

class FileDownloader
{
public:
	static auto download(	const std::string& url, 
							const std::string& fileName ) -> bool;
};