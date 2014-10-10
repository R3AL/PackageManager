#pragma once

#include <string>

#include "Settings.hpp"

class Profile
{
	private:
		std::string	m_name;
		Settings	m_settings;

	public:
		Profile( const std::string& profileName );

		auto name()									const	-> std::string;
		auto save()									const	-> void;
		auto settings()								const	-> const Settings&;

		auto load( std::string profileName = "" )			-> void;
};