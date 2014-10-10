#pragma once

#include <type_traits>
#include <typeinfo>

template <typename From>
struct converter
{
	const From& from;

	converter( const From& from ): from(from) {}

	template <typename To>
	To to() const
	{
		return static_cast< To >( from );
	}
};

template <typename From>
converter<From> convert( const From& from )
{
	return converter<From>( from );
}