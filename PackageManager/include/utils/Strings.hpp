#pragma once

#include <vector>
#include <string>

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

	public:
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

			mutableStringProxy( std::string& str );

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

			operator std::string() const;

			immutableStringProxy( const std::string& str );

		private:
			std::string m_str;
		};


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