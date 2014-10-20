#pragma once

#include <string>
#include <unordered_map>

namespace utils
{
	namespace internal
	{
		template <typename T>
		struct proxy
		{
			const std::string& m_str;

			std::unordered_map< std::string, T > m_caseValueMapping;
			T m_default;
		
			proxy( const std::string& str ): m_str( str ) {}

			proxy& Case( const std::string& caseValue, const T& returnValue )
			{
				m_caseValueMapping[ caseValue ] = returnValue;

				return *this;
			}

			proxy& Default( const T& value )
			{
				m_default = value;

				return *this;
			}

			T eval() const
			{
				if( m_caseValueMapping.count( m_str ) )
				{
					return m_caseValueMapping.at( m_str );
				}

				return m_default;
			}
		};
	}

	template <typename T>
	internal::proxy<T> Switch( const std::string& str )
	{
		return internal::proxy<T>( str );
	}


	template <typename T>
	class has_callOperator
	{
	private:
		template <typename U, U>
		class check
		{ };
    
		template <typename C, typename R>
		static auto f(check<R (C::*)(), &C::operator()>*) -> char;

		template <typename C, typename R>
		static auto f(check<R (C::*)() const, &C::operator()>*) -> char;
    
		template <typename C, typename R>
		static auto f(...) -> long;
    
	public:
		static const bool value = (sizeof(f<T>(0)) == sizeof(char));
	};

	
	class proxy2;

	template <	typename T, 
				bool isCallable	= has_callOperator<T>::value>
	struct proxy3;

	// Non callable
	template <typename T>
	struct proxy3<T, false>
	{
		const proxy2* const						m_proxy2;
		std::unordered_map< std::string, T >	m_caseValueMapping;
		T										m_default;

		proxy3( const proxy2* const proxy2,
				const std::string& value,
				const T& rvalue ) :
			m_proxy2( proxy2 )
		{
			m_caseValueMapping[ value ] = rvalue;
		}

		proxy3& Case(	const std::string& value,
						const T& rvalue )
		{
			m_caseValueMapping[ value ] = rvalue;

			return *this;
		}

		proxy3& Default( const T& rvalue )
		{
			m_default = rvalue;

			return *this;
		}

		T Eval()
		{
			if( m_caseValueMapping.count( m_proxy2->m_str ) )
			{
				return m_caseValueMapping.at( m_proxy2->m_str );
			}

			return m_default;
		}
	};
	
	// Callable
	template <typename T>
	struct proxy3<T, true>
	{
		const proxy2* const						m_proxy2;
		std::unordered_map< std::string, T >	m_caseValueMapping;
		T										m_default;

		proxy3( const proxy2* const proxy2,
				const std::string& value,
				const T& rvalue ) :
			m_proxy2( proxy2 )
		{
			m_caseValueMapping[ value ] = rvalue;
		}

		proxy3& Case(	const std::string& value,
						const T& rvalue )
		{
			m_caseValueMapping[ value ] = rvalue;

			return *this;
		}

		proxy3& Default( const T& rvalue )
		{
			m_default = rvalue;

			return *this;
		}

		decltype( std::declval<T>()() ) Eval()
		{
			if( m_caseValueMapping.count( m_proxy2->m_str ) )
			{
				return m_caseValueMapping.at( m_proxy2->m_str )();
			}

			return m_default();
		}
	};


	class proxy2
	{
		const std::string& m_str;

	public:
		proxy2( const std::string& str ) :
			m_str( str )
		{

		}

		template <typename T>
		proxy3<T> Case(	const std::string& value,
						const T& rvalue )
		{
			return proxy3<T>( this, value, rvalue );
		}
	};

	static proxy2 Switch2( const std::string& str )
	{
		return proxy2( str );
	}
}