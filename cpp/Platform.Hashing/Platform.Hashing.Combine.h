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
    void Combine(std::size_t& hashAccumulator, const T* value, std::size_t length)
    {
        hashAccumulator = Internal::crc32(reinterpret_cast<const std::uint8_t*>(value), length * sizeof(T), hashAccumulator);
    }

    void Combine(std::size_t& hashAccumulator, auto&& value)
    {
        Combine(hashAccumulator, &value, 1);
    }
}
