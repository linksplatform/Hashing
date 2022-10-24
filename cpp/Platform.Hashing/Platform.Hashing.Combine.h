#pragma once

#include "Platform.Hashing.Crc32.h"

namespace Platform::Hashing
{
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
