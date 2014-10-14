#pragma once

#include <string>

#include "Settings.hpp"

class Profile
{
	friend class SettingsManager;

	private:
		std::string	m_name;
		Settings	m_settings;

		/*
			Only SettingsManager can create profiles
		*/
		Profile( const std::string& profileName );

	public:
		static auto getNull() -> const Profile&;

		auto name()		const	-> const	std::string&;
		auto name()				->			std::string&;
		auto settings()	const	-> const	Settings&;
		auto settings()			->			Settings&;
		auto save()		const	-> void;

		auto load( std::string profileName = "" ) -> void;
};