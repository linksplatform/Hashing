#pragma once

namespace Platform::Hashing
{
    constexpr std::size_t CombineHashes(std::size_t hash1, std::size_t hash2)
    {
        // https://stackoverflow.com/a/2595226
        hash1 ^= hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
        return hash1;
    }
}
