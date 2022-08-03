#pragma once

#include "crc32_impl.h"

namespace noexport {
template <typename Self>
concept trivial_hashable =
    (std::is_standard_layout_v<Self> and std::is_trivial_v<Self>) or std::is_fundamental_v<Self>;
}

namespace Platform::Hashing {

constexpr auto combine_hashes(uint64_t hash1, uint64_t hash2) noexcept -> uint64_t {
    // https://stackoverflow.com/a/2595226
    // possible integer overflows
    hash1 ^= hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
    return hash1;
}

void combine_to(uint64_t& accum, size_t hash) noexcept {
    accum = combine_hashes(accum, hash);
}

auto expand(uint64_t originalHash) noexcept -> uint64_t {
    // platform target requirement
    if constexpr (sizeof(uint64_t) == 8) {
        uint64_t add = 0;
        combine_hashes(add, originalHash);
        combine_hashes(add, add ^ originalHash);
        uint64_t hash = static_cast<uint64_t>(originalHash) | add << (sizeof(uint32_t) * 8);
        return hash;
    } else {
        return originalHash;
    }
}

void combine_crc32_unsafe(uint64_t& accum, const std::byte* ptr, size_t len) noexcept {
    auto hash = noexport::crc32(ptr, len, 0);
    accum = combine_hashes(accum, expand(hash));
}

template <noexport::trivial_hashable T>
void combine_crc32(uint64_t& accum, const T& val) noexcept {
    // SAFETY: any `trivial_hashable` type must be represented as [uint8_t; sizeof(T)]
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    const auto& bytes = *reinterpret_cast<const std::array<std::byte, sizeof(T)>*>(&val);
    combine_crc32_unsafe(accum, bytes.data(), sizeof(T));
}

}  // namespace Platform::Hashing
