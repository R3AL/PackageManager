#pragma once

#include <string>
#include <unordered_map>

namespace utils
{
	namespace
	{
		template <typename T>
		struct proxy
		{
			std::string& m_str;

			std::unordered_map< std::string, T > m_caseValueMapping;
			T m_default;
		
			proxy( std::string& str ): m_str( str ) {}

			proxy& Case( const std::string& caseValue, const T& returnValue )
			{
				m_caseValueMapping[ caseValue ] = returnValue;

				return *this;
			}

			proxy& Default( const T& value )
			{
				m_default = value;

				return *this;
			}

			T eval() const
			{
				if( m_caseValueMapping.count( m_str ) )
				{
					return m_caseValueMapping[ m_str ];
				}

				return m_default;
			}
		};
	}

	template <typename T>
	proxy<T> Switch( std::string& str )
	{
		return proxy<T>( str );
	}
}