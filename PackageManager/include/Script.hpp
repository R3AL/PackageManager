#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class Library;

enum class Section
{
	PreInstall,
	PostInstall,
	Install,
	Remove
};

class Script
{
	bool m_valid;

	typedef std::unordered_map< std::string, std::string > SectionContent;

	std::unordered_map< Section, 
						SectionContent > m_sections;
public:
	Script( const Library* const library );

	auto hasSection(const Section& section) const	-> bool;
	auto section(const Section& section)	const	-> const	SectionContent&;
	auto section(const Section& section)			->			SectionContent&;

	auto run()								const	-> void;
};