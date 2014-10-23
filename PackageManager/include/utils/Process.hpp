#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <tuple>

#include "utils/Metafunctions.hpp"

enum Channel
{
    StdOutput = 1,
    StdError,
	Null
};

enum RedirectionType
{
    Default,
    Append
};

class Process
{
private:
    const std::string					m_process;
    mutable std::vector< std::string >	m_arguments;

    mutable std::unordered_map< Channel,
            std::tuple< std::string, bool, bool > > m_redirects;

    template <typename T>
    auto arg_internal(	const T& arg,
                        std::true_type hasStdToString,
                        std::true_type hasInsertionOperator ) const -> const Process&
    {
        m_arguments.push_back( std::to_string( arg ) );

        return *this;
    }

    template <typename T>
    auto arg_internal(	const T& arg,
                        std::true_type	hasStdToString,
                        std::false_type doesNotHaveInsertionOperator ) const -> const Process&
    {
        m_arguments.push_back( std::to_string( arg ) );

        return *this;
    }

    template <typename T>
    auto arg_itnernal(	const T& arg,
                        std::false_type doesNotHaveStdToString,
                        std::true_type	hasInsertionOperator ) const -> const Process&
    {
        std::stringstream ss;
        std::string argStr;

        ss << arg;
        ss >> argStr;

        m_arguments.push_back( argStr );

        return *this;
    }

    template <typename T>
    auto arg_internal(	const T& arg,
                        std::false_type,
                        std::false_type ) const -> const Process&
    {
        static_assert( false, "Input argument cannot be stringified !" );
    }

public:
    Process( const std::string& process );

    template <typename ...Args>
    Process( const std::string& process, Args&&... args ) :
        m_process( process )
    {
        arg( args... );
    }

    auto arg( const std::string& arg )	const -> const Process&;
    auto arg( const char* arg )			const -> const Process&;

    template <typename T>
    auto arg( const T& arg ) const -> const Process&
    {
        return arg_internal(arg,
                            std::integral_constant<bool, metafunctions::has_stdToString<T>::value>::type(),
                            std::integral_constant<bool, metafunctions::has_insertionOperator<T>::value>::type() );
    }

    template <typename T, typename ...Args>
    auto arg( const T& first, Args&&... rest ) const -> const Process&
    {
        arg(first);
        arg(rest...);

        return *this;
    }

    auto redirect( const Channel& channel, const std::string& to, const RedirectionType& type = Default )	const -> const Process&;
    auto redirect( const Channel& channel, const Channel& to )												const -> const Process&;

    auto run() const -> int;

    static const Process Copy;
    static const Process CopyFolder;
    static const Process Remove;
};
