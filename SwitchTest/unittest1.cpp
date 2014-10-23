#include <SDKDDKVer.h>

#include "CppUnitTest.h"
#include "CppUnitTestAssert.h"

#include <string>

#include "../PackageManager/include/utils/Switch.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SwitchTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(NonCallableType)
		{
			using namespace utils;

			std::string str = "5";

			auto val = Switch( str ).Case("1", 1)
									.Case("2", 2)
									.Case("3", 3)
									.Case("4", 4)
									.Case("5", 5)
									.Default(6)
									.Eval();

			Assert::AreEqual(val, 5);
		}

		TEST_METHOD(CallableType)
		{
			using namespace utils;

			std::string str = "6";

			auto val = Switch( str ).Case("1", []{ return 1; })
									.Case("2", []{ return 2; })
									.Case("3", []{ return 3; })
									.Case("4", []{ return 4; })
									.Case("5", []{ return 5; })
									.Default([]{ return 6; })
									.Eval();

			Assert::AreEqual(val, 6);
		}
	};
}