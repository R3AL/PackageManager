#pragma once

#include <string>
#include <unordered_map>
#include <functional> // std::function

#include "Metafunctions.hpp"

namespace utils
{
	namespace internal
	{
		class firstLevelProxy
		{
			template <	typename T, 
						bool isCallable = metafunctions::has_callOperator<T>::value>
			friend struct secondLevelProxy;

			const std::string& m_str;

		public:
			firstLevelProxy( const std::string& str ) :
				m_str( str )
			{

			}

			template <typename T>
			auto Case(	const std::string&	value,
						const T&			rvalue ) -> secondLevelProxy<T>
			{
				return secondLevelProxy<T>( this, value, rvalue );
			}
		};

		// Non callable
		template <typename T>
		struct secondLevelProxy<T, false>
		{
			const firstLevelProxy* const			m_firstLevelProxy;
			std::unordered_map< std::string, T >	m_caseValueMapping;
			T										m_default;

			secondLevelProxy(	const firstLevelProxy* const	firstLevelProxy,
								const std::string&				value,
								const T&						rvalue ) :
				m_firstLevelProxy( firstLevelProxy )
			{
				m_caseValueMapping[ value ] = rvalue;
			}

			auto Case(	const std::string&	value, 
						const T&			rvalue ) -> secondLevelProxy&
			{
				m_caseValueMapping[ value ] = rvalue;

				return *this;
			}

			auto Default( const T& rvalue ) -> secondLevelProxy&
			{
				m_default = rvalue;

				return *this;
			}

			auto Eval() -> T
			{
				if( m_caseValueMapping.count( m_firstLevelProxy->m_str ) )
				{
					return m_caseValueMapping.at( m_firstLevelProxy->m_str );
				}

				return m_default;
			}
		};

		// Callable
		template <typename T>
		struct secondLevelProxy<T, true>
		{
			/*
			 *	Lambdas must be wrapped in a std::function since different identical lambdas are of different types
			 *
			 *	Example:
			 *		auto a = []{};
			 *		auto b = []{};
			 *		
			 *	The type of 'a' is not the same as the type of 'b'
			 */
			typedef typename metafunctions::returnTypeOfCall<T>::type	return_type;
			typedef std::function< return_type() >						caseValueCallableType;

			const firstLevelProxy* const								m_firstLevelProxy;
			std::unordered_map< std::string, caseValueCallableType >	m_caseValueMapping;
			caseValueCallableType										m_default;

			secondLevelProxy(	const firstLevelProxy*	const	firstLevelProxy,
								const std::string&				value,
								const caseValueCallableType&	rvalue ) :
				m_firstLevelProxy( firstLevelProxy )
			{
				m_caseValueMapping[ value ] = rvalue;
			}

			auto Case(	const std::string& value,
						const caseValueCallableType& rvalue ) -> secondLevelProxy&
			{
				m_caseValueMapping[ value ] = rvalue;

				return *this;
			}

			auto Default( const caseValueCallableType& rvalue ) -> secondLevelProxy&
			{
				m_default = rvalue;

				return *this;
			}

			auto Eval() -> return_type
			{
				if( m_caseValueMapping.count( m_firstLevelProxy->m_str ) )
				{
					return m_caseValueMapping.at( m_firstLevelProxy->m_str )();
				}

				return m_default();
			}
		};
	}

	static auto Switch( const std::string& str ) -> internal::firstLevelProxy
	{
		return internal::firstLevelProxy( str );
	}
}