#pragma once

#include <cstring>
#include <functional>
#include <iostream>
#include <ranges>

namespace Platform::Hashing {

// fixme: at now `CrcHash` (DefaultHash) must be declared after `hash`/`hash_span`
template <typename T>
struct CrcHash {};

template <TTA Hasher = CrcHash, typename T>
constexpr auto hash(const T& val) noexcept(noexport::noexcept_with<Hasher, T>) -> uint64_t {
    return Hasher<T>{}(val);
}

template <TTA Hasher, std::size_t... Idx>
constexpr auto combine_tuple(const auto& tp, std::index_sequence<Idx...> /*unused*/) noexcept(
    false /* wrapper must guarantee noexcept (see `hash`) */) -> uint64_t {
    uint64_t accum = 0;
    ((combine_to(accum, hash<Hasher>(std::get<Idx>(tp)))), ...);
    return accum;
}

template <TTA Hasher = CrcHash, typename... Args>
constexpr auto hash(const Args&... args) noexcept(noexport::noexcept_with<Hasher, Args...>)
    -> uint64_t {
    return combine_tuple<Hasher>(std::tie(args...), std::make_index_sequence<sizeof...(args)>{});
}

template <typename T, TTA Hasher>
struct hash_span_fn {
    template <size_t N>
    // `std::span<const T>` is very stupid
    constexpr auto operator()(std::span</*const */ T, N> span) const
        noexcept(noexport::noexcept_with<Hasher, T>) -> uint64_t {
        uint64_t accum = 0;
        for (auto&& piece : span) {
            combine_to(accum, hash<Hasher>(piece));
        }
        return accum;
    }
};

template <TTA Hasher = CrcHash, typename T, size_t N>
constexpr auto hash_span(std::span<T, N> span) noexcept(hash_span_fn<T, Hasher>{}(span)) {
    return hash_span_fn<T, Hasher>{}(span);
}

}  // namespace Platform::Hashing
