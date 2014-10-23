#pragma once

#include <string>
#include <vector>

#ifdef _WIN32
	#include <filesystem>
#else
	#include <experimental/filesystem>
#endif

namespace utils
{
	namespace Filesystem
	{
		static auto isDirectory(const std::string& fsPath)	-> bool;
		static auto isFile(const std::string& fsPath)		-> bool;

		/*
		 *	Returns the current working directory path
		 *	('pwd' equivalent)
		 */
		static auto currentPath()									-> std::string;

		/*
		 *	Changes the current working directory
		 *	('cd' equivalent)
		 *	Returns the new curentpath
		 */
		static auto changeCurrentPath(const std::string& fsPath)	-> std::string;

		/*
		 *	Runs an UnaryFunctor on every folder and file recursively starting from fsPath
		 */
		template <typename UnaryFunctor>
		static auto recursiveVisit(	const std::string& fsPath,
									const UnaryFunctor& functor) -> void
		{
			using namespace std::tr2::sys;

			for(auto	it = recursive_directory_iterator(path( fsPath ));
						it != recursive_directory_iterator();
						++it)
			{
				const auto& file = it->path();

				functor(file);
			}
		}


		/*
		 *	Special empty tags for the specialization of 'recursiveFind'
		 */
		struct FirstResult{};
		struct AllResults{};

		namespace internal
		{
			template <typename T>
			struct stopStyleReturn;

			template <>
			struct stopStyleReturn<FirstResult>
			{
				typedef std::string type;
			};

			template <>
			struct stopStyleReturn<AllResults>
			{
				typedef std::vector< std::string > type;
			};

			template <typename StopStyle>
			struct recursiveFindHelper;

			template <>
			struct recursiveFindHelper<FirstResult>
			{
				typedef std::string return_type;

				static auto find(	const std::string& fsPath,
									const std::string& name ) -> return_type
				{
					return_type rval;

					recursiveVisit( fsPath,
									[&rval, &name]( const std::string& fsPath )
									{
										if( rval.empty() )
										{
											auto filePath = std::tr2::sys::path(fsPath);

											if( filePath.filename() == name )
											{
												rval = filePath.string();
											}
										}
									});

					return rval;
				}
			};

			template <>
			struct recursiveFindHelper<AllResults>
			{
				typedef std::vector< std::string > return_type;

				static auto find(	const std::string& fsPath,
									const std::string& name ) -> return_type
				{
					return_type rval;

					recursiveVisit( fsPath,
									[&rval, &name]( const std::string& fsPath )
									{
										auto filePath = std::tr2::sys::path(fsPath);

										if( filePath.filename() == name )
										{
											rval.push_back( filePath.string() );
										}
									});

					return rval;
				}
			};
		}

		/*
		 *	Searcher recursively for a file/folder in a given path.
		 *	This function can be specialized with 2 special tags 'FirstResult' or 'AllResults'.
		 *	The return type depends on this specialization:
		 *		for 'FirstResult' the return type will be a std::string
		 *		for 'AllResults' the return type will be a std::vector< std::string >
		 */
		template <	typename StopStyle, 
					typename return_type = internal::stopStyleReturn<StopStyle>::type>
		static auto recursiveFind(	const std::string& fsPath,
									const std::string& name ) -> return_type
		{
			return internal::recursiveFindHelper<StopStyle>::find( fsPath, name );
		}

	}
}