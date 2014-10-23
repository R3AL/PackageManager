#pragma once

#include "utils/Metafunctions.hpp"

namespace internal
{
	template <	typename T,
				bool isPointer	= std::is_pointer<T>::value,
				bool hasGetNull = std::is_class<T>::value && metafunctions::has_getNull<T>::value>
	struct nullGetter;

	// pointer
	template <typename T>
	struct nullGetter<T, true, false>
	{
		static auto getNull() -> T
		{
			return T(nullptr);
		}
	};
	// nullGetter<T, true, true> should never happen

	template <typename T>
	struct nullGetter<T, false, false>
	{
		static auto getNull() -> T
		{
			static_assert( false, "Type has no null representation !" );
		}
	};

	template <typename T>
	struct nullGetter<T, false, true>
	{
		static auto getNull() -> T
		{
			return T::getNull();
		}
	};

	class nulltype
	{
	public:
		template <typename T>
		operator T() const
		{
			return nullGetter<T>::getNull();
		}

		template <typename T>
		auto as() const -> T
		{
			return static_cast<T>( *this );
		}
	};

	template <	typename T, 
				bool isClassStruct	= std::is_class<T>::value,
				bool hasGetNull		= metafunctions::has_getNull<T>::value>
	struct comparator;

	// not a class/struct
	template <typename T>
	struct comparator<T, false, false>
	{
		static auto isNull(const T& obj) -> bool
		{
			return false;
		}
	};
	// comparator<T, false, true> should never happen


	// class/struct without getNull function
	template <typename T>
	struct comparator<T, true, false>
	{
		static auto isNull(const T& obj) -> bool
		{
			static_assert( false, "Type does not support null !" );
		}
	};

	// class/struct with getNull function
	template <typename T>
	struct comparator<T, true, true>
	{
		static auto isNull(const T& obj) -> bool
		{
			return obj == T::getNull();
		}
	};

	// pointer type of class/struct
	template <typename T>
	struct comparator<T*, true, false>
	{
		static auto isNull(const T* obj) -> bool
		{
			return obj == nullptr;
		}
	};
	// comparator<T*, true, true> should never happen


	// pointer type of fundamental type
	template <typename T>
	struct comparator<T*, false, false>
	{
		static auto isNull(const T* obj) -> bool
		{
			return comparator<T*, true>::isNull( obj );
		}
	};
	// comparator<T*, false, true> should never happen
}

template <typename T>
auto operator==( const T& obj, const internal::nulltype& nullObj ) -> bool
{
	return internal::comparator<T>::isNull( obj );
}

template <typename T>
auto operator==( const internal::nulltype& nullObj, const T& obj ) -> bool
{
	return obj == nullObj;
}

template <typename T>
auto operator!=( const T& obj, const internal::nulltype& nullObj ) -> bool
{
	return !( obj == nullObj );
}

template <typename T>
auto operator!=( const internal::nulltype& nullObj, const T& obj ) -> bool
{
	return obj != nullObj;
}

static const internal::nulltype null;