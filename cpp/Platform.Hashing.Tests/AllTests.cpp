#include "pch.h"

#include <gtest/gtest.h>
#include <Platform.Hashing.h>
#include <list>
#include <queue>

namespace Platform::Hashing::Tests
{
    TEST(CombineTest, Trivial)
    {
        std::size_t hash1 = 0;
        Combine(hash1, 42);
        ASSERT_NE(0U, hash1);
        std::size_t hash2 = 0;
        Combine(hash2, 42);
        ASSERT_EQ(hash2, hash1);
    }

    TEST(HashingAnyTest, Basic)
    {
        ASSERT_EQ(Hash(std::any{1}), Hash(1));
        ASSERT_EQ(Hash(std::any{"1"}), Hash("1"));
        ASSERT_EQ(Hash(std::any{u"1"}), Hash(u"1"));
        ASSERT_EQ(Hash(std::any{u8"1"}), Hash(u8"1"));
        ASSERT_EQ(Hash(std::any{U"1"}), Hash(U"1"));
        ASSERT_EQ(Hash(std::any{L"1"}), Hash(L"1"));
    }

    TEST(HashingTest, Basic)
    {
        std::size_t hash1 = Hash(42);
        ASSERT_NE(0, hash1);
        std::size_t hash2 = Hash(42);
        ASSERT_EQ(hash2, hash1);

        {
            int a[]{ 1, 2, 3 };
            int b[]{ 1, 2, 3 };
            ASSERT_EQ(Hash(a), Hash(b));
        }

        {
            auto a = "Platform.Hashing";
            auto b = "Platform.Hashing";
            ASSERT_EQ(Hash(a), Hash(b));
        }
    }

    TEST(HashingTest, Args)
    {
        std::size_t hash1 = Hash(1, 2);
        ASSERT_NE(static_cast<std::size_t>(0), hash1);
        std::size_t hash2 = Hash(1, 2);
        ASSERT_EQ(hash2, hash1);
    }

    TEST(HashingTest, Tuple)
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

    TEST(HashingTest, CustomStruct)
    {
        struct DataType
        {
            int a, b, c, d;
        };

        {
            std::size_t hash1 = Hash(DataType{});
            std::size_t hash2 = Hash(DataType{});
            ASSERT_EQ(hash1, hash2);
        }
        {
            std::size_t hash1 = Hash(DataType{ 1, 1, 1, 1 });
            std::size_t hash2 = Hash(DataType{ 1, 2, 3, 4 });
            ASSERT_NE(hash1, hash2);
        }
        {
            auto object1 = DataType{ 1, 1, 1, 1 };
            auto object2 = object1;

            std::size_t hash1 = Hash(object1);
            std::size_t hash2 = Hash(object2);
            ASSERT_EQ(hash1, hash2);
        }
        {
            std::size_t hash1 = Hash(DataType{ 1, 1, 1, 1 });
            std::size_t hash2 = Hash(DataType{ 1, 2, 3, 4 });
            ASSERT_NE(hash1, hash2);
        }
    }

    TEST(HashingTest, HashingRange)
    {
        // Hash will allow you to hash any collection for which it is valid:
        //  - std::ranges::data()
        //  - std::ranges::size()

        {
            std::vector<std::list<int>> a{{ 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }};
            std::vector<std::list<int>> b{{ 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }};
            ASSERT_EQ(Hash(a), Hash(b));
        }

        {
            auto a = std::vector{ 1, 2, 3, 4 };
            auto b = std::tuple{ 1, 2, 3, 4 };

            std::size_t hash1 = Hash(a);
            std::size_t hash2 = Hash(b);
            ASSERT_NE(hash1, hash2);
        }

        {
            auto a = std::vector{ 1, 2, 3, 4 };
            auto b = std::array{ 1, 2, 3, 4 };
            auto c = std::list{ 1, 2, 3, 4 };
            auto d = std::deque{ 1, 2, 3, 4 };

            std::size_t hash1 = Hash(a);
            std::size_t hash2 = Hash(b);
            std::size_t hash3 = Hash(b);
            std::size_t hash4 = Hash(b);
            ASSERT_EQ(hash1, hash2);
            ASSERT_EQ(hash2, hash3);
            ASSERT_EQ(hash3, hash4);
        }

        {
            auto a = std::map<int, int>{{ 1, 1 }, { 2, 2 }, { 3, 3 }};
            auto b = std::map<int, int>{{ 1, 1 }, { 2, 2 }, { 3, 3 }};

            auto c = std::unordered_map<int, int>{{ 1, 1 }, { 2, 2 }, { 3, 3 }};
            auto d = std::unordered_map<int, int>{{ 1, 1 }, { 2, 2 }, { 3, 3 }};

            std::size_t hash1 = Hash(a);
            std::size_t hash2 = Hash(b);

            std::size_t hash3 = Hash(a);
            std::size_t hash4 = Hash(b);
            ASSERT_EQ(hash1, hash2);
            ASSERT_EQ(hash3, hash4);

            ASSERT_EQ(hash1, hash3);
        }

        {
            auto a = std::array<std::string, 4>{ "i", "z", "x", "l" };
            auto b = std::array<std::string, 4>{ "i", "z", "x", "l" };

            std::size_t hash1 = Hash(a);
            std::size_t hash2 = Hash(b);
            ASSERT_EQ(hash1, hash2);
        }
    }

    TEST(ProcessorArchitectureTest, FallbackImplementationConsistency)
    {
        const std::uint8_t testData[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
        const std::size_t dataSize = sizeof(testData);
        const std::size_t initialHash = 0x12345678;
        
        std::size_t fallbackResult = Platform::Hashing::Internal::crc32fallback(testData, dataSize, initialHash);
        ASSERT_NE(0U, fallbackResult);
        
        std::size_t fallbackResult2 = Platform::Hashing::Internal::crc32fallback(testData, dataSize, initialHash);
        ASSERT_EQ(fallbackResult, fallbackResult2);
    }

#ifdef _X86_64_
    TEST(ProcessorArchitectureTest, X86_64ImplementationsConsistency)
    {
        const std::uint8_t testData[] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
            0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
        };
        const std::size_t dataSize = sizeof(testData);
        const std::size_t initialHash = 0;
        
        std::size_t fallbackResult = Platform::Hashing::Internal::crc32fallback(testData, dataSize, initialHash);
        std::size_t sseWithoutPclmulResult = Platform::Hashing::Internal::crc32sse_without_pclmul(testData, dataSize, initialHash);
        
        ASSERT_NE(0U, fallbackResult);
        ASSERT_NE(0U, sseWithoutPclmulResult);
        
        std::size_t sseWithPclmulResult = Platform::Hashing::Internal::crc32sse_with_pclmul(testData, dataSize, initialHash);
        ASSERT_NE(0U, sseWithPclmulResult);
    }

    TEST(ProcessorArchitectureTest, X86_64LargeDataConsistency)
    {
        std::vector<std::uint8_t> largeData(1024);
        for (std::size_t i = 0; i < largeData.size(); ++i) {
            largeData[i] = static_cast<std::uint8_t>(i & 0xFF);
        }
        
        const std::size_t initialHash = 0x87654321;
        
        std::size_t fallbackResult = Platform::Hashing::Internal::crc32fallback(largeData.data(), largeData.size(), initialHash);
        std::size_t sseWithoutPclmulResult = Platform::Hashing::Internal::crc32sse_without_pclmul(largeData.data(), largeData.size(), initialHash);
        std::size_t sseWithPclmulResult = Platform::Hashing::Internal::crc32sse_with_pclmul(largeData.data(), largeData.size(), initialHash);
        
        ASSERT_NE(0U, fallbackResult);
        ASSERT_NE(0U, sseWithoutPclmulResult);
        ASSERT_NE(0U, sseWithPclmulResult);
    }
#endif

#ifdef _AARCH_
    TEST(ProcessorArchitectureTest, ARMImplementationsConsistency)
    {
        const std::uint8_t testData[] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
            0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
        };
        const std::size_t dataSize = sizeof(testData);
        const std::size_t initialHash = 0;
        
        std::size_t fallbackResult = Platform::Hashing::Internal::crc32fallback(testData, dataSize, initialHash);
        std::size_t armCrc32Result = Platform::Hashing::Internal::crc32_pclmul_vmull_p64_crc32(testData, dataSize, initialHash);
        std::size_t armPolyfillResult = Platform::Hashing::Internal::crc32_pclmul_vmull_p64_polyfill_crc32(testData, dataSize, initialHash);
        
        ASSERT_NE(0U, fallbackResult);
        ASSERT_NE(0U, armCrc32Result);
        ASSERT_NE(0U, armPolyfillResult);
    }

    TEST(ProcessorArchitectureTest, ARMLargeDataConsistency)
    {
        std::vector<std::uint8_t> largeData(1024);
        for (std::size_t i = 0; i < largeData.size(); ++i) {
            largeData[i] = static_cast<std::uint8_t>(i & 0xFF);
        }
        
        const std::size_t initialHash = 0x87654321;
        
        std::size_t fallbackResult = Platform::Hashing::Internal::crc32fallback(largeData.data(), largeData.size(), initialHash);
        std::size_t armCrc32Result = Platform::Hashing::Internal::crc32_pclmul_vmull_p64_crc32(largeData.data(), largeData.size(), initialHash);
        std::size_t armPolyfillResult = Platform::Hashing::Internal::crc32_pclmul_vmull_p64_polyfill_crc32(largeData.data(), largeData.size(), initialHash);
        
        ASSERT_NE(0U, fallbackResult);
        ASSERT_NE(0U, armCrc32Result);
        ASSERT_NE(0U, armPolyfillResult);
    }
#endif

    TEST(ProcessorArchitectureTest, RuntimeDispatchConsistency)
    {
        const std::uint8_t testData[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11};
        const std::size_t dataSize = sizeof(testData);
        
        std::size_t hash1 = 0;
        std::size_t hash2 = 0;
        
        Combine(hash1, testData, dataSize);
        Combine(hash2, testData, dataSize);
        
        ASSERT_EQ(hash1, hash2);
        ASSERT_NE(0U, hash1);
    }

    TEST(ProcessorArchitectureTest, EmptyDataHandling)
    {
        const std::uint8_t* emptyData = nullptr;
        const std::size_t emptySize = 0;
        const std::size_t initialHash = 0x12345678;
        
        std::size_t fallbackResult = Platform::Hashing::Internal::crc32fallback(emptyData, emptySize, initialHash);
        ASSERT_EQ(initialHash, fallbackResult);
        
#ifdef _X86_64_
        std::size_t sseWithoutPclmulResult = Platform::Hashing::Internal::crc32sse_without_pclmul(emptyData, emptySize, initialHash);
        ASSERT_EQ(initialHash, sseWithoutPclmulResult);
        
        std::size_t sseWithPclmulResult = Platform::Hashing::Internal::crc32sse_with_pclmul(emptyData, emptySize, initialHash);
        ASSERT_EQ(initialHash, sseWithPclmulResult);
#endif

#ifdef _AARCH_
        std::size_t armCrc32Result = Platform::Hashing::Internal::crc32_pclmul_vmull_p64_crc32(emptyData, emptySize, initialHash);
        ASSERT_EQ(initialHash, armCrc32Result);
        
        std::size_t armPolyfillResult = Platform::Hashing::Internal::crc32_pclmul_vmull_p64_polyfill_crc32(emptyData, emptySize, initialHash);
        ASSERT_EQ(initialHash, armPolyfillResult);
#endif
    }

    TEST(ProcessorArchitectureTest, SingleByteHandling)
    {
        const std::uint8_t singleByte = 0x42;
        const std::size_t initialHash = 0;
        
        std::size_t fallbackResult = Platform::Hashing::Internal::crc32fallback(&singleByte, 1, initialHash);
        ASSERT_NE(initialHash, fallbackResult);
        
#ifdef _X86_64_
        std::size_t sseWithoutPclmulResult = Platform::Hashing::Internal::crc32sse_without_pclmul(&singleByte, 1, initialHash);
        ASSERT_NE(initialHash, sseWithoutPclmulResult);
        
        std::size_t sseWithPclmulResult = Platform::Hashing::Internal::crc32sse_with_pclmul(&singleByte, 1, initialHash);
        ASSERT_NE(initialHash, sseWithPclmulResult);
#endif

#ifdef _AARCH_
        std::size_t armCrc32Result = Platform::Hashing::Internal::crc32_pclmul_vmull_p64_crc32(&singleByte, 1, initialHash);
        ASSERT_NE(initialHash, armCrc32Result);
        
        std::size_t armPolyfillResult = Platform::Hashing::Internal::crc32_pclmul_vmull_p64_polyfill_crc32(&singleByte, 1, initialHash);
        ASSERT_NE(initialHash, armPolyfillResult);
#endif
    }

    TEST(ProcessorArchitectureTest, UnalignedDataHandling)
    {
        std::uint8_t buffer[64];
        for (int i = 0; i < 64; ++i) {
            buffer[i] = static_cast<std::uint8_t>(i);
        }
        
        for (int offset = 0; offset < 8; ++offset) {
            const std::uint8_t* data = buffer + offset;
            const std::size_t dataSize = 32 - offset;
            const std::size_t initialHash = 0;
            
            std::size_t fallbackResult = Platform::Hashing::Internal::crc32fallback(data, dataSize, initialHash);
            ASSERT_NE(0U, fallbackResult);
            
#ifdef _X86_64_
            std::size_t sseWithoutPclmulResult = Platform::Hashing::Internal::crc32sse_without_pclmul(data, dataSize, initialHash);
            ASSERT_NE(0U, sseWithoutPclmulResult);
            
            std::size_t sseWithPclmulResult = Platform::Hashing::Internal::crc32sse_with_pclmul(data, dataSize, initialHash);
            ASSERT_NE(0U, sseWithPclmulResult);
#endif

#ifdef _AARCH_
            std::size_t armCrc32Result = Platform::Hashing::Internal::crc32_pclmul_vmull_p64_crc32(data, dataSize, initialHash);
            ASSERT_NE(0U, armCrc32Result);
            
            std::size_t armPolyfillResult = Platform::Hashing::Internal::crc32_pclmul_vmull_p64_polyfill_crc32(data, dataSize, initialHash);
            ASSERT_NE(0U, armPolyfillResult);
#endif
        }
    }
}
