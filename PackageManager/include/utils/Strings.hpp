#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <initializer_list>

namespace utils
{
	class Strings
	{
		static auto split(	const std::string& str, 
							const char& delimiter )					-> std::vector< std::string >;

		static auto replace(const std::string& str, 
							const char& charToFind, 
							const char& charToReplaceWith )			-> std::string;

		static auto replace(const std::string& str,
							const std::string& strToFind,
							const std::string& strToReplaceWith )	-> std::string;
			
		static auto toLower(const std::string& str)					-> std::string;
		static auto toUpper(const std::string& str)					-> std::string;
		static auto strip(	std::string str,
							const char& charToStrip)				-> std::string;
		static auto trimRight(	const std::string& str,
								const std::size_t& count)			-> std::string;

		class mutableStringProxy
		{
		public:
			auto replace(	const char& charToReplace,
							const char& charToReplaceWith)					-> mutableStringProxy;
			auto replace(	const std::string& strToReplace,
							const std::string& strToReplaceWith)			-> mutableStringProxy;
			auto tolower()													-> mutableStringProxy;
			auto toupper()													-> mutableStringProxy;
			auto split( const char& delimiter )						const	-> std::vector< std::string >;
			auto contains( const std::string& what )				const	-> bool;
			auto strip(const char& charToStrip)								-> mutableStringProxy;
			auto strip(std::initializer_list<char> list )					-> mutableStringProxy;
			auto trimRight( const std::size_t& count )						-> mutableStringProxy;

			auto operator==( const mutableStringProxy& )			const	-> bool;
			auto operator!=( const mutableStringProxy& )			const	-> bool;

			mutableStringProxy( std::string& str );

			template <typename T>
			auto as() const -> T
			{
				T val;

				std::stringstream oss( m_str );
				oss >> val;

				return val;
			}

			/*
			 *	This method is useful when passing in strings to STL algorithms
			 *	to disambiguate which comparison operator is to be used
			 */
			auto str()				const -> std::string;
			operator std::string&() const;

		private:
			std::string& m_str;
		};


		class immutableStringProxy
		{
		public:
			auto replace(	const char& charToReplace,
							const char& charToReplaceWith)			const -> immutableStringProxy;
			auto replace(	const std::string& strToReplace,
							const std::string& strToReplaceWith)	const -> immutableStringProxy;
			auto tolower()											const -> immutableStringProxy;
			auto toupper()											const -> immutableStringProxy;
			auto split( const char& delimiter )						const -> std::vector< std::string >;
			auto contains( const std::string& what )				const -> bool;
			auto strip(const char& charToStrip)						const -> immutableStringProxy;
			auto strip(std::initializer_list<char> list )			const -> immutableStringProxy;
			auto trimRight( const std::size_t& count )				const -> immutableStringProxy;
			
			auto operator==( const immutableStringProxy& )			const -> bool;
			auto operator!=( const immutableStringProxy& )			const -> bool;

			template <typename T>
			auto as() const -> T
			{
				T val;

				std::stringstream oss( m_str );
				oss >> val;

				return val;
			}

			/*
			 *	This method is useful when passing in strings to STL algorithms
			 *	to disambiguate which comparison operator is to be used
			 */
			auto str()				const -> std::string;
			operator std::string()	const;

			immutableStringProxy( const std::string& str );

		private:
			std::string m_str;
		};

		friend std::ostream& operator<<( std::ostream&, const Strings::mutableStringProxy& );
		friend std::ostream& operator<<( std::ostream&, const Strings::immutableStringProxy& );

	public:
		/*
			all operations will be performed on the input string
		*/
		static auto Mutate( std::string& str )		-> mutableStringProxy;

		/*
			all operations will be performed on a copy of the input string
		*/
		static auto Keep( const std::string& str )	-> immutableStringProxy;
	};

	auto operator<<( std::ostream& os, const Strings::mutableStringProxy&	mutableStringProxy )	-> std::ostream&;
	auto operator<<( std::ostream& os, const Strings::immutableStringProxy& immutableStringProxy )	-> std::ostream&;
}