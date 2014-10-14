#pragma once

namespace internal
{
	class nulltype
	{
	};

	template <typename T>
	struct comparator
	{
		static bool isNull(const T& obj)
		{
			return false;
		}
	};

	template <typename T>
	struct comparator<T*>
	{
		static bool isNull(const T* obj)
		{
			return true;
		}
	};
}

template <typename T>
bool operator==( const T& obj, const internal::nulltype& nullObj )
{
	return internal::comparator<T>::isNull( obj );
}

template <typename T>
bool operator==( const internal::nulltype& nullObj, const T& obj )
{
	return obj == nullObj;
}

template <typename T>
bool operator!=( const T& obj, const internal::nulltype& nullObj )
{
	return !( obj == nullObj );
}

template <typename T>
bool operator!=( const internal::nulltype& nullObj, const T& obj )
{
	return obj != nullObj;
}

static const internal::nulltype null;