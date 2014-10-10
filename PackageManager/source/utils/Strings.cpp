#include "utils/Strings.hpp"

#include <sstream>
#include <algorithm>
#include <cctype> // std::tolower, std::toupper

namespace utils
{
	auto Strings::split(	const std::string& str, 
							const char& delimiter ) -> std::vector< std::string >
	{
		std::vector<std::string> elems;

		std::stringstream ss( str );
		std::string item;

		while( std::getline(ss, item, delimiter) ) 
		{
			elems.push_back(item);
		}

		return elems;
	}

	auto Strings::replace(	const std::string& str, 
							const char& charToFind, 
							const char& charToReplaceWith ) -> std::string
	{
		auto strCopy = str;

		std::transform( str.begin(),
						str.end(),
						strCopy.begin(),
						[&]( char ch )
						{
							if( ch == charToFind )
								return charToReplaceWith;

							return ch;
						});

		return strCopy;
	}

	auto Strings::replace(	const std::string& str,
							const std::string& strToFind,
							const std::string& strToReplaceWith ) -> std::string
	{
		auto strCopy = str;

		for(std::size_t pos = 0; ;pos += strToReplaceWith.length())
		{
			pos = strCopy.find( strToFind, pos );

			if( pos == std::string::npos )
			{
				break;
			}

			strCopy.erase( pos, strToFind.length() );
			strCopy.insert( pos, strToReplaceWith );
		}

		return strCopy;
	}

	auto Strings::toLower(const std::string& str) -> std::string
	{
		auto strCopy = str;

		std::transform( str.begin(),
						str.end(),
						strCopy.begin(),
						[]( const char& ch )
						{
							return std::tolower( ch );
						});

		return strCopy;
	}

	auto Strings::toUpper(const std::string& str) -> std::string
	{
		auto strCopy = str;

		std::transform( str.begin(),
						str.end(),
						strCopy.begin(),
						[]( const char& ch )
						{
							return std::toupper( ch );
						});

		return strCopy;
	}



	/*
		Mutable
	*/
	auto Strings::Mutate( std::string& str ) -> mutableStringProxy
	{
		return mutableStringProxy( str );
	}

	Strings::mutableStringProxy::mutableStringProxy( std::string& str ) : m_str(str)
	{

	}

	Strings::mutableStringProxy::operator std::string&() const
	{
		return m_str;
	}

	auto operator<<(std::ostream& os, 
					const Strings::mutableStringProxy& mutableStringProxy ) -> std::ostream&
	{
		os << static_cast< std::string >( mutableStringProxy );

		return os;
	}

	auto Strings::mutableStringProxy::replace(	const char& charToReplace,
												const char& charToReplaceWith) -> mutableStringProxy
	{
		m_str = Strings::replace(	m_str,
									charToReplace,
									charToReplaceWith );

		return *this;
	}

	auto Strings::mutableStringProxy::replace(	const std::string& strToReplace,
												const std::string& strToReplaceWith) -> mutableStringProxy
	{
		m_str = Strings::replace(	m_str,
									strToReplace,
									strToReplaceWith );

		return *this;
	}
	
	auto Strings::mutableStringProxy::tolower()	-> mutableStringProxy
	{
		m_str = Strings::toLower( m_str );

		return *this;
	}

	auto Strings::mutableStringProxy::toupper()	-> mutableStringProxy
	{
		m_str = Strings::toUpper( m_str );

		return *this;
	}

	auto Strings::mutableStringProxy::split( const char& delimiter ) const -> std::vector< std::string >
	{
		return Strings::split( m_str, delimiter );
	}




	/*
		Immutable
	*/
	auto Strings::Keep( const std::string& str ) -> immutableStringProxy
	{
		return immutableStringProxy( str );
	}

	Strings::immutableStringProxy::immutableStringProxy( const std::string& str ) : m_str(str)
	{

	}
	
	Strings::immutableStringProxy::operator std::string() const
	{
		return m_str;
	}

	auto operator<<(std::ostream& os, 
					const Strings::immutableStringProxy& immutableStringProxy ) -> std::ostream&
	{
		os << static_cast< std::string >( immutableStringProxy );
		
		return os;
	}

	auto Strings::immutableStringProxy::replace(const char& charToReplace,
												const char& charToReplaceWith) const -> immutableStringProxy
	{
		return immutableStringProxy( Strings::replace(	m_str,
														charToReplace,
														charToReplaceWith ) );
	}

	auto Strings::immutableStringProxy::replace(const std::string& strToReplace,
												const std::string& strToReplaceWith) const -> immutableStringProxy
	{
		return immutableStringProxy( Strings::replace(	m_str,
														strToReplace,
														strToReplaceWith ) );
	}

	auto Strings::immutableStringProxy::tolower() const -> immutableStringProxy
	{
		return immutableStringProxy( Strings::toLower( m_str ) );
	}

	auto Strings::immutableStringProxy::toupper() const -> immutableStringProxy
	{
		return immutableStringProxy( Strings::toUpper( m_str ) );
	}

	auto Strings::immutableStringProxy::split( const char& delimiter ) const -> std::vector< std::string >
	{
		return Strings::split( m_str, delimiter );
	}
}