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
		}

		TEST_METHOD(HashTest)
		{
			std::size_t hash1 = Hash(42);
			Assert::AreNotEqual(static_cast<std::size_t>(0), hash1);
			std::size_t hash2 = Hash(42);
			Assert::AreEqual(hash2, hash1);
		}

		TEST_METHOD(HashArgsTest)
		{
			std::size_t hash1 = Hash(1, 2);
			Assert::AreNotEqual(static_cast<std::size_t>(0), hash1);
			std::size_t hash2 = Hash(1, 2);
			Assert::AreEqual(hash2, hash1);
		}

		TEST_METHOD(TupleHashingTest)
		{
			auto zero = static_cast<std::size_t>(0);
			std::tuple<int, int> tuple = { 1, 2 };
			std::hash<std::tuple<int, int>> hasher;
			std::size_t hash1 = hasher(tuple);
			Assert::AreNotEqual(zero, hash1);
			std::size_t hash2 = hasher(tuple);
			Assert::AreEqual(hash2, hash1);
			std::size_t hash3 = Hash(1, 2);
			Assert::AreEqual(hash2, hash3);
			std::size_t hash4 = Hash(std::make_tuple(1, 2));
			Assert::AreEqual(hash4, hash3);
			std::size_t hash5 = Hash(tuple);
			Assert::AreEqual(hash5, hash3);
			std::size_t hash6 = Hash<std::tuple<int, int>>({ 1, 2 });
			Assert::AreEqual(hash6, hash3);
			std::size_t hash7 = Hash(tuple, tuple);
			Assert::AreNotEqual(zero, hash7);
		}
	};
}
