#pragma once

#ifndef PLATFORM_HASHING_COMBINE
#define PLATFORM_HASHING_COMBINE

#include <crc32c.c>
#include <cstdint>

namespace Platform::Hashing
{
    template<typename T>
    void Combine(std::uint32_t& hashAccumulator, T&& value)
    {
        std::size_t size = sizeof(T);
        hashAccumulator = crc32c_append(hashAccumulator, reinterpret_cast<const uint8_t*>(&value), size);
    }

    template<typename T>
    void Combine(std::uint32_t& hashAccumulator, const T* value, std::size_t length)
    {
        std::size_t size = sizeof(T);
        hashAccumulator = crc32c_append(hashAccumulator, reinterpret_cast<const uint8_t*>(value), size * length);
    }

    constexpr std::size_t CombineHash(std::size_t hash1, std::size_t hash2)
    {
        // https://stackoverflow.com/a/2595226
        hash1 ^= hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
        return hash1;
    }
}

#endif
