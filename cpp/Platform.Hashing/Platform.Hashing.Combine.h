#pragma once

#include "Platform.Hashing.Crc32.h"

namespace Platform::Hashing
{
    constexpr std::size_t CombineHashes(std::size_t hash1, std::size_t hash2)
    {
        // https://stackoverflow.com/a/2595226
        hash1 ^= hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
        return hash1;
    }

    template<typename T>
    void Combine(std::uint32_t& hashAccumulator, const T* value, std::size_t length)
    {
    #if defined(__x86_64__) || defined(_M_AMD64)
        using Internal::AMDCrc32::crc32;
    #else
        using Internal::IntelCrc32::crc32;
    #endif

        constexpr std::size_t size = sizeof(T);
        hashAccumulator = CombineHashes(hashAccumulator, crc32(reinterpret_cast<const std::uint8_t*>(value), size * length, 0));
    }

    void Combine(std::uint32_t& hashAccumulator, auto&& value)
    {
        Combine(hashAccumulator, &value, 1);
    }
}
