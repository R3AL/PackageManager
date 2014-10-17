#pragma once

#include <chrono>
#include <functional>
#include <type_traits>
#include <future>
#include <atomic>

#include "utils/FormattedPrint.hpp"

namespace utils
{
	namespace
	{
		static const char slashes[4] = 
		{
			'/',
			'-',
			'\\',
			'|'
		};
	}

	class ProgressIndicator
	{
		class progressIndicatorThreadWrapper
		{
			std::thread			m_thread;
			std::atomic_bool	m_stopCondition;

		public:
			progressIndicatorThreadWrapper( const std::chrono::nanoseconds& updateInterval )
			{
				m_stopCondition.store( false );
				
				m_thread = std::thread( [this, &updateInterval]
										{
											FormattedPrint::On(std::cout, false).app("    ");
											
											int slashIndex = 0;

											while( ! m_stopCondition.load() )
											{
												FormattedPrint::On(std::cout, false).app("\b\b\b \b")
																					.color( Green )
																					.app('[')
																					.color( Yellow )
																					.app( slashes[ slashIndex++ ] )
																					.color( Green )
																					.app(']')
																					.color();

												slashIndex = slashIndex % 4;

												std::this_thread::sleep_for( updateInterval );
											}

											FormattedPrint::On(std::cout, false).app("\b\b\b");
										});
			}

			~progressIndicatorThreadWrapper()
			{
				m_stopCondition.store( true );

				if( m_thread.joinable() )
				{
					m_thread.join();
				}
			}
		};

		public:

			/*
			 *	This function runs the given function/functor/lambda in the current thread and displays an ascii progres indicator using a new thread.
			 *	This also returns the value returned by the given function/functor/lambda
			 */
			template <	typename T, 
						typename R = decltype( std::declval<T>()() ) >
			static auto Task(	const T& task,
								const std::chrono::nanoseconds& updateInterval = std::chrono::milliseconds(100) ) -> R
			{
				progressIndicatorThreadWrapper progress( updateInterval );

				return task();
			}
	};
}