#include "utils/FormattedPrint.hpp"

namespace utils
{
	auto FormattedPrint::On( std::ostream& os, const bool& usePrefix ) -> streamProxy
	{
		if( usePrefix )
		{
			os << " --- ";
		}

		return streamProxy( os );
	}

	FormattedPrint::streamProxy::streamProxy( std::ostream& os ):
		m_os( &os )
#ifdef _WIN32
		,m_console( GetStdHandle( STD_OUTPUT_HANDLE ) )
#endif
	{

	}

	auto FormattedPrint::streamProxy::endl() const -> streamProxy
	{
		(*m_os) << std::endl;

		return *this;
	}

	auto FormattedPrint::streamProxy::color( const Color& color ) const -> streamProxy
	{
		#ifdef _WIN32
			SetConsoleTextAttribute(m_console, 
									static_cast<WORD>( color ));
		#endif

		return *this;
	}
}