#pragma once

#include <string>

class Library
{
public:
	enum class FileType
	{
		Archive,
		SingleHeaderH,
		SingleHeaderHPP,

		Unknown
	};

	enum class ArchiveType
	{
		RAR,
		ZIP,
		TAR_GZ,
		TAR_BZ2,
		TAR_Z,
		TAR_XZ,

		Unknown
	};

	Library(const std::string& name,
			const std::string& url);

	auto name()		const -> std::string;
	auto url()		const -> std::string;
	auto type()		const -> FileType;
	auto archType() const -> ArchiveType;
	auto isValid()	const -> bool;
	auto install()	const -> bool;

private:
	auto identifyType()											-> void;
	auto archiveTypeByIndex(const std::size_t& index)	const	-> ArchiveType;
	auto extensionStringFromUrl()						const	-> std::string;
	auto download()										const	-> bool;

	static const std::string extensions[];

	const std::string m_name;
	const std::string m_url;

	bool m_valid;

	/*
	 *	Type of the file pointed by m_url
	 */
	FileType m_type;

	/*
	 *	Type of archive if m_type = Archive
	 */
	ArchiveType m_archtype;
};