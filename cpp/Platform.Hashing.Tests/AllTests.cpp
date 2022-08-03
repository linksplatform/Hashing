
#include <Platform.Hashing.h>
#include <gtest/gtest.h>

#include <list>

namespace Platform::Hashing::Tests {
TEST(CombineTest, Trivial) {
    std::uint64_t hash1 = 0;
    combine_crc32(hash1, 42);
    ASSERT_NE(0U, hash1);
    std::uint64_t hash2 = 0;
    combine_crc32(hash2, 42);
    ASSERT_EQ(hash2, hash1);
}

TEST(HashingTest, Basic) {
    uint64_t hash1 = hash(42);
    ASSERT_NE(0, hash1);
    uint64_t hash2 = hash(42);
    ASSERT_EQ(hash2, hash1);

    {
        std::array a = {1, 2, 3};
        std::array b = {1, 2, 3};
        ASSERT_EQ(hash_span(std::span(a)), hash_span(std::span(b)));
        ASSERT_EQ((hash_span<int32_t, std::hash<int32_t>>(std::span(a))),
                  (hash_span<int32_t, std::hash<int32_t>>(std::span(b))));
    }

    {
        auto a = std::span("Platform.Hashing");
        auto b = std::span("Platform.Hashing");
        ASSERT_EQ(hash_span(a), hash_span(b));
    }
}

TEST(HashingTest, Args) {
    uint64_t hash1 = hash(1, 2);
    ASSERT_NE(hash1, 0);
    uint64_t hash2 = hash(1, 2);
    ASSERT_EQ(hash2, hash1);
}

TEST(HashingTest, Tuple) {
    std::tuple<int, int> tuple = {1, 2};
    CrcHash<std::tuple<int, int>> hasher;
    uint64_t hash1 = hasher(tuple);
    ASSERT_NE(hash1, 0);
    uint64_t hash2 = hasher(tuple);
    ASSERT_EQ(hash2, hash1);
    uint64_t hash3 = hash(1, 2);
    ASSERT_EQ(hash2, hash3);
    uint64_t hash4 = hash(std::make_tuple(1, 2));
    ASSERT_EQ(hash4, hash3);
    uint64_t hash5 = hash(tuple);
    ASSERT_EQ(hash5, hash3);
    uint64_t hash6 = hash<std::tuple<int, int>>({1, 2});
    ASSERT_EQ(hash6, hash3);
    uint64_t hash7 = hash(tuple, tuple);
    ASSERT_NE(hash7, 0);
}

TEST(HashingTest, CustomStruct) {
    struct DataType {
        int a, b, c, d;
    };

    {
        uint64_t hash1 = hash(DataType{});
        uint64_t hash2 = hash(DataType{});
        ASSERT_EQ(hash1, hash2);
    }
    {
        uint64_t hash1 = hash(DataType{1, 1, 1, 1});
        uint64_t hash2 = hash(DataType{1, 2, 3, 4});
        ASSERT_NE(hash1, hash2);
    }
    {
        auto object1 = DataType{1, 1, 1, 1};
        auto object2 = object1;

        uint64_t hash1 = hash(object1);
        uint64_t hash2 = hash(object2);
        ASSERT_EQ(hash1, hash2);
    }
    {
        uint64_t hash1 = hash(DataType{1, 1, 1, 1});
        uint64_t hash2 = hash(DataType{1, 2, 3, 4});
        ASSERT_NE(hash1, hash2);
    }
}

TEST(HashingTest, HashingRange) {
    {
        auto a = std::vector{1, 2, 3, 4};
        auto b = std::tuple{1, 2, 3, 4};

        uint64_t hash1 = hash_span(std::span(a));
        uint64_t hash2 = hash(b);
        ASSERT_NE(hash1, hash2);
    }

    {
        auto a = std::vector{1, 2, 3, 4};
        auto b = std::array{1, 2, 3, 4};

        ASSERT_EQ(hash_span(std::span(a)), hash_span(std::span(b)));
    }

    {
        auto a = std::array<std::string, 4>{"i", "z", "x", "l"};
        auto b = std::array<std::string, 4>{"i", "z", "x", "l"};

        uint64_t hash1 = hash_span(std::span(a));
        uint64_t hash2 = hash_span(std::span(b));
        ASSERT_EQ(hash1, hash2);
    }
}
}  // namespace Platform::Hashing::Tests
