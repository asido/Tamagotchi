#include "stdafx.h"

#include "StringUtilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EngineTests
{		
	TEST_CLASS(StringUtilitiesTest)
	{
		TEST_METHOD(WildcardMatchTest)
		{
			Assert::IsTrue(StringUtilities::WildcardMatch("*.xml", "File.xml"), L"1");
			Assert::IsFalse(StringUtilities::WildcardMatch("*.xml", "File.jpg"), L"2");

			Assert::IsFalse(StringUtilities::WildcardMatch("*.png", "File.png.jpg"), L"3");
			Assert::IsTrue(StringUtilities::WildcardMatch("File.png", "File.png"), L"4");
			Assert::IsTrue(StringUtilities::WildcardMatch("*File.png", "ElifFile.png"), L"5");
		}
	};
}