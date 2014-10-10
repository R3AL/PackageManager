#pragma once

#include <iostream>

#ifdef _WIN32
	#include <Windows.h>
#endif

namespace utils
{
	class FormattedPrint
	{
	public:
		template <typename T>
		static auto cout( const T& arg, const bool& addNewLine = false ) -> void
		{
			std::cout << " --- " << arg;
		
			if( addNewLine )
			{
				std::cout << std::endl;
			}
		}

		class streamProxy
		{
			std::ostream* m_os;

		public:

			streamProxy( std::ostream& os );

			template <typename T>
			auto app( const T& arg ) const -> streamProxy
			{
				(*m_os) << " --- " << arg;

				return streamProxy( *m_os );
			}

		};

		static auto On( std::ostream& os ) -> streamProxy;
	};
}