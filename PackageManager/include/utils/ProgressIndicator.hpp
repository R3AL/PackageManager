#pragma once

#include <chrono>
#include <functional>
#include <type_traits>

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
		public:
			template <	typename T, 
						typename R		= decltype( std::declval<T>()() ),
						typename IsVoid = std::is_same< void, R >::type >
			static R Task(	const T& task,
							const std::chrono::nanoseconds& updateInterval = std::chrono::milliseconds(100) )
			{
				return taskInternal(task, 
									updateInterval,
									IsVoid() );
			}

		private:
			template <typename T>
			static void taskInternal(	const T& task, 
										const std::chrono::nanoseconds& updateInterval, 
										std::true_type )
			{
				task();
			}

			template <	typename T, 
						typename R = decltype( std::declval<T>()() )>
			static R taskInternal(	const T& task, 
									const std::chrono::nanoseconds& updateInterval, 
									std::false_type )
			{
				return task();
			}
	};
}