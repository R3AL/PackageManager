#pragma once

#include <iostream>

#ifdef _WIN32
	#include <Windows.h>
#endif

namespace utils
{
	enum Color : unsigned short
	{
		Black = 0,
		DarkBlue,
		DarkGreen,
		DarkRed,
		DarkPurple,
		DarkYellow,
		LightBlue,
		Grey,
		DarkGrey,
		Blue,
		Green,
		Aqua,
		Red,
		Purple,
		Yellow,
		White
	};

	class FormattedPrint
	{
		class streamProxy
		{
			std::ostream* const m_os;

			#ifdef _WIN32
				HANDLE const m_console;
			#endif

		public:
			streamProxy( std::ostream& os );

			template <typename T>
			auto app( const T& arg ) const -> streamProxy
			{
				(*m_os) << arg;

				return *this;
			}

			auto endl()								const -> streamProxy;
			auto color( const Color& color = Grey ) const -> streamProxy;
		};

	public:
		static auto On( std::ostream& os, const bool& usePrefix = true ) -> streamProxy;
	};
}