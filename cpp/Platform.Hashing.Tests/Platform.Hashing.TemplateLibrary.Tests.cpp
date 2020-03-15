#include "pch.h"
#include "CppUnitTest.h"

#include <Platform.Hashing.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform::Hashing;

namespace PlatformHashingTemplateLibraryTests
{
	TEST_CLASS(PlatformHashingTemplateLibraryTests)
	{
	public:
		
		TEST_METHOD(CombineTest)
		{
			std::uint32_t hash1 = 0;
			Combine(hash1, 42);
			Assert::AreNotEqual(0U, hash1);
			std::uint32_t hash2 = 0;
			Combine(hash2, 42);
			Assert::AreEqual(hash2, hash1);
			std::uint32_t hash3 = Hash(42);
			Assert::AreEqual(hash2, hash3);
		}

		TEST_METHOD(HashTest)
		{
			std::uint32_t hash1 = Hash(42);
			Assert::AreNotEqual(0U, hash1);
			std::uint32_t hash2 = Hash(42);
			Assert::AreEqual(hash2, hash1);
		}

		TEST_METHOD(HashArgsTest)
		{
			std::uint32_t hash1 = Hash(1, 2);
			Assert::AreNotEqual(0U, hash1);
			std::uint32_t hash2 = Hash(1, 2);
			Assert::AreEqual(hash2, hash1);
		}

		TEST_METHOD(TupleHashingTest)
		{
			std::tuple<int, int> tuple = { 1, 2 };
			std::hash<std::tuple<int, int>> hasher;
			std::size_t hash1 = hasher(tuple);
			Assert::AreNotEqual(static_cast<std::size_t>(0), hash1);
			std::size_t hash2 = hasher(tuple);
			Assert::AreEqual(hash2, hash1);
			std::size_t hash3 = Hash(1, 2);
			Assert::AreEqual(hash2, hash3);
		}
	};
}
