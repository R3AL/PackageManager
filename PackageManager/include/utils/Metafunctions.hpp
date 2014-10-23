#pragma once

#include <string>
#include <iostream> // std::ostream
#include <type_traits> // std::declval

namespace metafunctions
{
	/*
	 *	Checks if a given type T has operator()() ( note: no parameters ) defined
	 */
	template <typename T>
	class has_callOperator
	{
	private:
		template <typename U, U>
		class check
		{ };

		template <typename C>
		static auto f(check<decltype(C::operator()), &C::operator()>*) -> char;

		template <typename C>
		static auto f(...) -> long;

	public:
		static const bool value = (sizeof(f<T>(0)) == sizeof(char));
	};

	/*
	 *	Checks if a given type T has a to_string function defined in the 'std' namespace
	 */
	using std::to_string;

    template <typename T>
    void to_string(T);

    template <typename T>
    struct has_stdToString
    {
        static const bool value = ! std::is_same< void, decltype( to_string( std::declval<T>() ) )>::value;
    };

	/*
	 *	Checks if a given type T has operator<< defined
	 */
    typedef char no;
    typedef char yes[2];

    struct any_t
    {
        template <typename T>
        any_t( T const& );
    };

    auto operator<<(const std::ostream&,
                    const any_t& )	-> no;

    auto test( std::ostream& )		-> yes&;
    auto test( no )					-> no;

    template<typename T>
    struct has_insertionOperator
    {
    private:
        static std::ostream &s;
        static T const &t;

    public:
        static bool const value = sizeof( test(s << t) ) == sizeof( yes );
    };

	/*
	 *	Checks if a given type T has the function 'static const T& getNull()' defined
	 */
	template <typename T>
	class has_getNull
	{
	private:
		template <typename U, U>
		class check
		{ };
    
		template <typename C>
		static auto f(check<const C& (*)(), &C::getNull>*) -> char;
    
		template <typename C>
		static auto f(...) -> long;
    
	public:
		static const bool value = (sizeof(f<T>(0)) == sizeof(char));
	};

	/*
	 *	Provides a typedef representing the type of return value of T.operator()
	 */
	template <typename T>
	struct returnTypeOfCall
	{
		typedef decltype( std::declval<T>()() ) type;
	};
}