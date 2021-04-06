#include "pch.h"

#include <gtest/gtest.h>
#include <Platform.Hashing.h>

using namespace Platform::Hashing;

namespace PlatformHashingTemplateLibraryTests
{
    TEST(CombineTest, Trivial) {
        {
            std::uint32_t hash1 = 0;
            Combine(hash1, 42);
            ASSERT_NE(0U, hash1);
            std::uint32_t hash2 = 0;
            Combine(hash2, 42);
            ASSERT_EQ(hash2, hash1);
        }
    }

    TEST(HashTest, Basic)
    {
        std::size_t hash1 = Hash(42);
        ASSERT_NE(static_cast<std::size_t>(0), hash1);
        std::size_t hash2 = Hash(42);
        ASSERT_EQ(hash2, hash1);
    }

    TEST(HashTest, Args)
    {
        std::size_t hash1 = Hash(1, 2);
        ASSERT_NE(static_cast<std::size_t>(0), hash1);
        std::size_t hash2 = Hash(1, 2);
        ASSERT_EQ(hash2, hash1);
    }

    TEST(HashTest, Tuple)
    {
        auto zero = static_cast<std::size_t>(0);
        std::tuple<int, int> tuple = { 1, 2 };
        std::hash<std::tuple<int, int>> hasher;
        std::size_t hash1 = hasher(tuple);
        ASSERT_NE(zero, hash1);
        std::size_t hash2 = hasher(tuple);
        ASSERT_EQ(hash2, hash1);
        std::size_t hash3 = Hash(1, 2);
        ASSERT_EQ(hash2, hash3);
        std::size_t hash4 = Hash(std::make_tuple(1, 2));
        ASSERT_EQ(hash4, hash3);
        std::size_t hash5 = Hash(tuple);
        ASSERT_EQ(hash5, hash3);
        std::size_t hash6 = Hash<std::tuple<int, int>>({ 1, 2 });
        ASSERT_EQ(hash6, hash3);
        std::size_t hash7 = Hash(tuple, tuple);
        ASSERT_NE(zero, hash7);
    }

    TEST(HashTest, Other)
    {
        {
            auto a = std::vector{1, 2, 3, 4};
            auto b = std::tuple{1, 2, 3, 4};

            std::size_t hash1 = Hash(a);
            std::size_t hash2 = Hash(b);
            ASSERT_NE(hash1, hash2);
        }
        {
            auto a = std::vector{1, 2, 3, 4};
            auto b = std::array{1, 2, 3, 4};

            std::size_t hash1 = Hash(a);
            std::size_t hash2 = Hash(b);
            ASSERT_NE(hash1, hash2);
        }
        {
            auto a = std::map<int, int>{{1, 1}, {2, 2}, {3, 3}};
            auto b = std::map<int, int>{{1, 1}, {2, 2}, {3, 3}};

            std::size_t hash1 = Hash(a);
            std::size_t hash2 = Hash(b);
            ASSERT_EQ(hash1, hash2);
        }
        {
            auto a = std::array<std::string, 4>{"i", "z", "x", "l"};
            auto b = std::array<std::string, 4>{"i", "z", "x", "l"};

            std::size_t hash1 = Hash(a);
            std::size_t hash2 = Hash(b);
            ASSERT_EQ(hash1, hash2);
        }
    }

    TEST(HashTest, CustomStruct)
    {
        struct DataType {int a, b, c, d;};

        {
            std::size_t hash1 = Hash(DataType{});
            std::size_t hash2 = Hash(DataType{});
            ASSERT_EQ(hash1, hash2);
        }
        {
            std::size_t hash1 = Hash(DataType{1, 1, 1, 1});
            std::size_t hash2 = Hash(DataType{1, 2, 3, 4});
            ASSERT_NE(hash1, hash2);
        }
        {
            auto object1 = DataType{1, 1, 1, 1};
            auto object2 = object1;

            std::size_t hash1 = Hash(object1);
            std::size_t hash2 = Hash(object2);
            ASSERT_EQ(hash1, hash2);

            hash1 = std::hash<DataType&>{}(object1);
            hash2 = std::hash<DataType&>{}(object2);
            ASSERT_NE(hash1, hash2);
        }
        {
            std::size_t hash1 = Hash(DataType{1, 1, 1, 1});
            std::size_t hash2 = Hash(DataType{1, 2, 3, 4});
            ASSERT_NE(hash1, hash2);
        }
    }
}

// If IDE not supported Google Test
/*
    int main()
    {
        ::testing::InitGoogleTest();
        RUN_ALL_TESTS();
    };
*/