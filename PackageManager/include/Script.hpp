#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <initializer_list>

class Library;

enum class Section
{
	Compilers,
	PreInstall,
	PostInstall,
	Install,
	Remove,

	Unknown
};

enum class ScriptCommand
{
	Print,
	Run,
	CopyFolder,
	Copy,
	Delete,

	Unknown
};

class Script
{
	const Library* const m_library;

	bool m_valid;

	typedef std::unordered_map< std::string, std::string > SectionContent;

	std::unordered_map< Section, 
						SectionContent > m_sections;

	auto runCommand(const std::string& command,
					const std::string& argument)					const -> void;
	auto runCommand(const std::string& command,
					std::initializer_list<std::string> arguments)	const -> void;
	auto runCommand(const std::string& command,
					const std::vector<std::string>& arguments)		const -> void;

public:
	Script( const Library* const library );

	auto hasSection(const Section& section) const	-> bool;
	auto section(const Section& section)	const	-> const	SectionContent&;
	auto section(const Section& section)			->			SectionContent&;

	auto run()								const	-> void;
};