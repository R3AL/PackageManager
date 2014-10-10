#include "utils/FormattedPrint.hpp"

namespace utils
{
	auto FormattedPrint::On( std::ostream& os ) -> streamProxy
	{
		return streamProxy( os );
	}

	FormattedPrint::streamProxy::streamProxy( std::ostream& os ):
		m_os( &os )
	{

	}
}